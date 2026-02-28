// BenchmarkExportBPLibrary.h

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BenchmarkExportBPLibrary.generated.h"

UCLASS()
class SKRIPSIIVAN_API UBenchmarkExportBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Buka folder/file path di Explorer
	UFUNCTION(BlueprintCallable, Category = "Benchmark|Export")
	static bool OpenFolderInExplorer(const FString& AbsFolderPath);

	// RunID otomatis dari waktu sekarang (biar folder unik)
	UFUNCTION(BlueprintPure, Category = "Benchmark|Export")
	static FString MakeRunIDNow();

	// Format float untuk CSV (pakai dot, pembulatan decimal)
	UFUNCTION(BlueprintPure, Category = "Benchmark|Export")
	static FString FloatToCsv(float Value, int32 Decimals = 3);

	// Ambil root autosave (prioritas dekat folder game, fallback Saved/)
	UFUNCTION(BlueprintPure, Category = "Benchmark|Export")
	static FString GetAutoBenchmarkRootAbs();

	// Simpan CSV+JSON ke Auto Folder: <AutoRoot>/<RunID>/
	UFUNCTION(BlueprintCallable, Category = "Benchmark|Export", meta = (DisplayName = "Save Benchmark Texts (Auto Folder)"))
	static bool SaveBenchmarkTexts_AutoFolder(
		const FString& RunID,
		const FString& CSVText,
		const FString& SummaryJSONText,
		bool bOpenFolderAfterSave,
		FString& OutFolderAbs,
		FString& OutCSVAbs,
		FString& OutJSONAbs
	);

	// Simpan CSV+JSON ke folder tertentu (parent folder) -> otomatis bikin subfolder BenchmarkReports/<RunID>/
	UFUNCTION(BlueprintCallable, Category = "Benchmark|Export", meta = (DisplayName = "Save Benchmark Texts To Folder"))
	static bool SaveBenchmarkTexts_ToFolder(
		const FString& RootFolderAbs,
		const FString& RunID,
		const FString& CSVText,
		const FString& SummaryJSONText,
		bool bOpenFolderAfterSave,
		FString& OutFolderAbs,
		FString& OutCSVAbs,
		FString& OutJSONAbs
	);

	// Pick folder (Windows native). Kalau cancel/gagal -> fallback auto.
	UFUNCTION(BlueprintCallable, Category = "Benchmark|Export", meta = (DisplayName = "Save Benchmark Texts Pick or Auto"))
	static bool SaveBenchmarkTexts_PickOrAuto(
		const FString& RunID,
		const FString& CSVText,
		const FString& SummaryJSONText,
		bool bTryPickFolder,
		bool bOpenFolderAfterSave,
		bool& bUsedPickedFolder,
		FString& OutFolderAbs,
		FString& OutCSVAbs,
		FString& OutJSONAbs
	);

	// Folder picker khusus Windows (native). Return false kalau cancel/gagal.
	UFUNCTION(BlueprintCallable, Category = "Benchmark|Export")
	static bool PickFolderDialog_Windows(const FString& DialogTitle, FString& OutFolderAbs);
};
