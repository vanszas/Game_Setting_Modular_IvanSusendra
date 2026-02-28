// BenchmarkExportBPLibrary.cpp

#include "BenchmarkExportBPLibrary.h"

#include "HAL/FileManager.h"
#include "HAL/PlatformProcess.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include <Windows.h>
#include <ShObjIdl.h>
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif

static FString NormalizeAbsDir(const FString& InDir)
{
	FString D = InDir;
	FPaths::NormalizeDirectoryName(D);
	D = FPaths::ConvertRelativePathToFull(D);
	return D;
}

static bool CanWriteToDir(const FString& AbsDir)
{
	const FString Dir = NormalizeAbsDir(AbsDir);
	IFileManager::Get().MakeDirectory(*Dir, true);

	const FString TestFile = FPaths::Combine(Dir, TEXT("__write_test.tmp"));
	const bool bOK = FFileHelper::SaveStringToFile(TEXT("ok"), *TestFile, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
	if (bOK)
	{
		IFileManager::Get().Delete(*TestFile, false, true, true);
	}
	return bOK;
}

static FString GetGameRootNearExe()
{
	// BaseDir biasanya: .../<GameName>/Binaries/Win64/
	const FString BaseDir = NormalizeAbsDir(FPlatformProcess::BaseDir());
	// Naik 2 folder: .../<GameName>/
	return NormalizeAbsDir(FPaths::Combine(BaseDir, TEXT(".."), TEXT("..")));
}

FString UBenchmarkExportBPLibrary::MakeRunIDNow()
{
	const FDateTime Now = FDateTime::Now();
	return Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
}

FString UBenchmarkExportBPLibrary::FloatToCsv(float Value, int32 Decimals)
{
	const double Scale = FMath::Pow(10.0, (double)Decimals);
	const double Rounded = FMath::RoundToDouble((double)Value * Scale) / Scale;
	return FString::SanitizeFloat(Rounded);
}

bool UBenchmarkExportBPLibrary::OpenFolderInExplorer(const FString& AbsFolderPath)
{
	FString Folder = AbsFolderPath;

	if (FPaths::FileExists(Folder))
	{
		Folder = FPaths::GetPath(Folder);
	}

	Folder = NormalizeAbsDir(Folder);

	if (!IFileManager::Get().DirectoryExists(*Folder))
	{
		return false;
	}

	FPlatformProcess::ExploreFolder(*Folder);
	return true;
}

FString UBenchmarkExportBPLibrary::GetAutoBenchmarkRootAbs()
{
	// Prioritas: folder game (dekat exe) -> <GameRoot>/BenchmarkReports
	// Kalau ga bisa nulis (contoh: Program Files), fallback ke Saved/BenchmarkReports
	FString RootPreferred;

	// Di editor, BaseDir itu engine folder, jadi mending Saved.
	// Tapi kamu bisa tetap pakai logic ini: cek writable saja.
	RootPreferred = FPaths::Combine(GetGameRootNearExe(), TEXT("BenchmarkReports"));
	if (CanWriteToDir(RootPreferred))
	{
		return NormalizeAbsDir(RootPreferred);
	}

	const FString Fallback = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("BenchmarkReports"));
	return NormalizeAbsDir(Fallback);
}

static bool SaveBenchmarkInternal(
	const FString& RootFolderAbs,
	const FString& RunID,
	const FString& CSVText,
	const FString& SummaryJSONText,
	bool bOpenFolderAfterSave,
	FString& OutFolderAbs,
	FString& OutCSVAbs,
	FString& OutJSONAbs
)
{
	const FString Root = NormalizeAbsDir(RootFolderAbs);

	// Struktur final: <Root>/<RunID>/
	const FString RunDir = NormalizeAbsDir(FPaths::Combine(Root, RunID));
	IFileManager::Get().MakeDirectory(*RunDir, true);

	const FString CSVPath = FPaths::Combine(RunDir, TEXT("samples.csv"));
	const FString JSONPath = FPaths::Combine(RunDir, TEXT("summary.json"));

	const bool bCSVOK = FFileHelper::SaveStringToFile(CSVText, *CSVPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
	const bool bJSONOK = FFileHelper::SaveStringToFile(SummaryJSONText, *JSONPath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);

	OutFolderAbs = RunDir;
	OutCSVAbs = CSVPath;
	OutJSONAbs = JSONPath;

	const bool bOK = bCSVOK && bJSONOK;

	if (bOK && bOpenFolderAfterSave)
	{
		UBenchmarkExportBPLibrary::OpenFolderInExplorer(RunDir);
	}

	return bOK;
}

bool UBenchmarkExportBPLibrary::SaveBenchmarkTexts_AutoFolder(
	const FString& RunID,
	const FString& CSVText,
	const FString& SummaryJSONText,
	bool bOpenFolderAfterSave,
	FString& OutFolderAbs,
	FString& OutCSVAbs,
	FString& OutJSONAbs
)
{
	const FString AutoRoot = GetAutoBenchmarkRootAbs();
	return SaveBenchmarkInternal(AutoRoot, RunID, CSVText, SummaryJSONText, bOpenFolderAfterSave, OutFolderAbs, OutCSVAbs, OutJSONAbs);
}

bool UBenchmarkExportBPLibrary::SaveBenchmarkTexts_ToFolder(
	const FString& RootFolderAbs,
	const FString& RunID,
	const FString& CSVText,
	const FString& SummaryJSONText,
	bool bOpenFolderAfterSave,
	FString& OutFolderAbs,
	FString& OutCSVAbs,
	FString& OutJSONAbs
)
{
	// Biar rapi, kita bikin subfolder BenchmarkReports dulu:
	const FString Root = NormalizeAbsDir(FPaths::Combine(RootFolderAbs, TEXT("BenchmarkReports")));
	if (!CanWriteToDir(Root))
	{
		// fallback auto
		return SaveBenchmarkTexts_AutoFolder(RunID, CSVText, SummaryJSONText, bOpenFolderAfterSave, OutFolderAbs, OutCSVAbs, OutJSONAbs);
	}

	return SaveBenchmarkInternal(Root, RunID, CSVText, SummaryJSONText, bOpenFolderAfterSave, OutFolderAbs, OutCSVAbs, OutJSONAbs);
}

bool UBenchmarkExportBPLibrary::PickFolderDialog_Windows(const FString& DialogTitle, FString& OutFolderAbs)
{
	OutFolderAbs.Empty();

#if PLATFORM_WINDOWS
	// Init COM
	const HRESULT HrInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	const bool bNeedUninit = (HrInit == S_OK || HrInit == S_FALSE);

	IFileDialog* FileDialog = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&FileDialog));
	if (SUCCEEDED(hr) && FileDialog)
	{
		DWORD Options = 0;
		FileDialog->GetOptions(&Options);
		FileDialog->SetOptions(Options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);

		if (!DialogTitle.IsEmpty())
		{
			FileDialog->SetTitle(*DialogTitle);
		}

		// Owner window = nullptr (aman, tanpa dependency DesktopPlatform/Slate)
		hr = FileDialog->Show(nullptr);
		if (SUCCEEDED(hr))
		{
			IShellItem* Item = nullptr;
			if (SUCCEEDED(FileDialog->GetResult(&Item)) && Item)
			{
				PWSTR WidePath = nullptr;
				if (SUCCEEDED(Item->GetDisplayName(SIGDN_FILESYSPATH, &WidePath)) && WidePath)
				{
					OutFolderAbs = FString(WidePath);
					CoTaskMemFree(WidePath);
				}
				Item->Release();
			}
		}

		FileDialog->Release();
	}

	if (bNeedUninit)
	{
		CoUninitialize();
	}

	return !OutFolderAbs.IsEmpty();
#else
	return false;
#endif
}

bool UBenchmarkExportBPLibrary::SaveBenchmarkTexts_PickOrAuto(
	const FString& RunID,
	const FString& CSVText,
	const FString& SummaryJSONText,
	bool bTryPickFolder,
	bool bOpenFolderAfterSave,
	bool& bUsedPickedFolder,
	FString& OutFolderAbs,
	FString& OutCSVAbs,
	FString& OutJSONAbs
)
{
	bUsedPickedFolder = false;

	FString RootFolder;

	if (bTryPickFolder)
	{
		FString Picked;
		if (PickFolderDialog_Windows(TEXT("Select Folder to Save Benchmark"), Picked))
		{
			RootFolder = Picked;
			bUsedPickedFolder = true;
		}
	}

	if (!RootFolder.IsEmpty())
	{
		// simpan ke folder yang dipilih
		return SaveBenchmarkTexts_ToFolder(RootFolder, RunID, CSVText, SummaryJSONText, bOpenFolderAfterSave, OutFolderAbs, OutCSVAbs, OutJSONAbs);
	}

	// fallback auto
	return SaveBenchmarkTexts_AutoFolder(RunID, CSVText, SummaryJSONText, bOpenFolderAfterSave, OutFolderAbs, OutCSVAbs, OutJSONAbs);
}
