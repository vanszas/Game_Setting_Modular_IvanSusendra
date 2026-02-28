#include "CPUInfoBPLibrary.h"

#include "HAL/PlatformMisc.h"
#include "HAL/PlatformTime.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

namespace
{
	// State sampler (global untuk proses)
	static bool   GHasPrev = false;
	static uint64 GPrevProcTime100ns = 0; // kernel+user dalam 100ns units (FILETIME)
	static double GPrevWallSeconds = 0.0;

	static uint64 FileTimeToUInt64(const FILETIME& Ft)
	{
		return (uint64(Ft.dwHighDateTime) << 32) | uint64(Ft.dwLowDateTime);
	}
}

FString UCPUInfoBPLibrary::GetCPUName()
{
	// UE menyediakan GetCPUBrand di PlatformMisc. :contentReference[oaicite:1]{index=1}
	return FPlatformMisc::GetCPUBrand();
}

void UCPUInfoBPLibrary::ResetProcessCPUUsageSampler()
{
	GHasPrev = false;
	GPrevProcTime100ns = 0;
	GPrevWallSeconds = 0.0f;
}

bool UCPUInfoBPLibrary::GetProcessCPUUsagePercent(float& OutCPUPercent)
{
	OutCPUPercent = 0.0f;

#if !PLATFORM_WINDOWS
	// Kalau nanti kamu butuh cross-platform, kita buat implementasi alternatif.
	return false;
#else
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	const HANDLE ProcHandle = GetCurrentProcess();

	if (!GetProcessTimes(ProcHandle, &CreationTime, &ExitTime, &KernelTime, &UserTime))
	{
		return false;
	}

	const uint64 Kernel100ns = FileTimeToUInt64(KernelTime);
	const uint64 User100ns = FileTimeToUInt64(UserTime);
	const uint64 Proc100ns = Kernel100ns + User100ns; // dijumlah semua thread :contentReference[oaicite:2]{index=2}

	const double NowWall = FPlatformTime::Seconds();

	// First sample: set baseline dulu
	if (!GHasPrev)
	{
		GHasPrev = true;
		GPrevProcTime100ns = Proc100ns;
		GPrevWallSeconds = NowWall;
		return false; // belum bisa hitung % karena belum ada delta
	}

	const uint64 DeltaProc100ns = (Proc100ns >= GPrevProcTime100ns) ? (Proc100ns - GPrevProcTime100ns) : 0;
	const double DeltaWall = NowWall - GPrevWallSeconds;

	// Update baseline
	GPrevProcTime100ns = Proc100ns;
	GPrevWallSeconds = NowWall;

	if (DeltaWall <= 0.0)
	{
		return false;
	}

	const double DeltaProcSeconds = double(DeltaProc100ns) * 1e-7; // 100ns -> seconds
	const int32  LogicalCores = FMath::Max(1, FPlatformMisc::NumberOfCoresIncludingHyperthreads());

	// Normalisasi core supaya 0..100% (mirip Task Manager per-process)
	const double CpuPercent = (DeltaProcSeconds / DeltaWall) * (100.0 / double(LogicalCores));

	OutCPUPercent = float(FMath::Clamp(CpuPercent, 0.0, 100.0));
	return true;

#endif
}
