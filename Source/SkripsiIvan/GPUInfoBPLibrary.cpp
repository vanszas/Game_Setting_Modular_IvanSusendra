#include "GPUInfoBPLibrary.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <pdh.h>
#include <pdhmsg.h>
#include <vector>
#include <string>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#if PLATFORM_WINDOWS
static PDH_HQUERY G_GPUQuery = nullptr;
static TArray<PDH_HCOUNTER> G_GPUCounters;
static bool G_bGPUInited = false;
static bool G_bOnly3D = true;

FString UGPUInfoBPLibrary::GetGPUName()
{
	return FPlatformMisc::GetPrimaryGPUBrand();
}

static bool BuildGPUCounters()
{
	// Bersihin dulu

	if (G_GPUQuery)
	{
		PdhCloseQuery(G_GPUQuery);
		G_GPUQuery = nullptr;
	}
	G_GPUCounters.Empty();

	PDH_STATUS s = PdhOpenQueryW(nullptr, 0, &G_GPUQuery);
	if (s != ERROR_SUCCESS || !G_GPUQuery)
	{
		return false;
	}

	const DWORD Pid = GetCurrentProcessId();

	// Wildcard: ambil semua engine counter milik PID kita
	// Contoh instance yg kamu lihat: GPU Engine(pid_12408_luid_..._engtype_3D)\Utilization Percentage
	std::wstring Wild =
		L"\\GPU Engine(pid_" + std::to_wstring(Pid) + L"_*)\\Utilization Percentage";

	// 1) Minta size buffer dulu
	DWORD BufSize = 0;
	s = PdhExpandWildCardPathW(nullptr, Wild.c_str(), nullptr, &BufSize, 0);
	if (s != PDH_MORE_DATA || BufSize == 0)
	{
		PdhCloseQuery(G_GPUQuery);
		G_GPUQuery = nullptr;
		return false;
	}

	// 2) Ambil list counter path (MULTI_SZ)
	std::vector<wchar_t> Buffer;
	Buffer.resize(BufSize);

	s = PdhExpandWildCardPathW(nullptr, Wild.c_str(), Buffer.data(), &BufSize, 0);
	if (s != ERROR_SUCCESS)
	{
		PdhCloseQuery(G_GPUQuery);
		G_GPUQuery = nullptr;
		return false;
	}

	// 3) Add counter satu-satu
	for (wchar_t* p = Buffer.data(); *p; p += wcslen(p) + 1)
	{
		std::wstring Path = p;

		// Optional: hanya engtype_3D biar lebih “game relevant”
		if (G_bOnly3D)
		{
			if (Path.find(L"engtype_3D") == std::wstring::npos)
				continue;
		}

		PDH_HCOUNTER Counter = nullptr;
		s = PdhAddCounterW(G_GPUQuery, Path.c_str(), 0, &Counter);
		if (s == ERROR_SUCCESS && Counter)
		{
			G_GPUCounters.Add(Counter);
		}
	}

	// Kalau gak ada yang ketemu, gagal beneran
	if (G_GPUCounters.Num() == 0)
	{
		PdhCloseQuery(G_GPUQuery);
		G_GPUQuery = nullptr;
		return false;
	}

	// Prime sample (penting! counter rate butuh minimal 2 sample)
	PdhCollectQueryData(G_GPUQuery);

	return true;
}
#endif

void UGPUInfoBPLibrary::ResetGPUProcessUsageSampler(bool bOnly3DEngine)
{
#if PLATFORM_WINDOWS
	G_bOnly3D = bOnly3DEngine;
	G_bGPUInited = BuildGPUCounters();
#else
	(void)bOnly3DEngine;
#endif
}

bool UGPUInfoBPLibrary::GetGPUProcessUsagePercent(float& OutGPUPercent)
{
	OutGPUPercent = 0.f;

#if PLATFORM_WINDOWS
	// Kalau belum init, coba build
	if (!G_bGPUInited || !G_GPUQuery || G_GPUCounters.Num() == 0)
	{
		G_bGPUInited = BuildGPUCounters();
		if (!G_bGPUInited)
			return false; // ini FAILED yg “bener”
	}

	// Collect data terbaru
	PDH_STATUS s = PdhCollectQueryData(G_GPUQuery);
	if (s != ERROR_SUCCESS)
	{
		return false;
	}

	double Sum = 0.0;
	int32 Valid = 0;

	for (PDH_HCOUNTER C : G_GPUCounters)
	{
		PDH_FMT_COUNTERVALUE V;
		DWORD Type = 0;
		s = PdhGetFormattedCounterValue(C, PDH_FMT_DOUBLE, &Type, &V);

		// Jangan gampang-gampang fail. Kalau invalid, skip counter itu.
		if (s == ERROR_SUCCESS && V.CStatus == ERROR_SUCCESS)
		{
			Sum += V.doubleValue;
			Valid++;
		}
	}

	// Kalau counter ada tapi nilainya 0, itu NORMAL (idle).
	// Jadi selama query & counters valid, return TRUE.
	OutGPUPercent = (Valid > 0) ? (float)Sum : 0.f;

	// Catatan: kalau kamu sum beberapa engine, bisa >100.
	// Untuk display, clamp aja biar gak aneh.
	OutGPUPercent = FMath::Clamp(OutGPUPercent, 0.f, 100.f);

	return true;
#else
	return false;
#endif
}
