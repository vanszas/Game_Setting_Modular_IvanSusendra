// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VRAMBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SKRIPSIIVAN_API UVRAMBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// TotalVRAM = Dedicated video memory (kapasitas)
	// UsedVRAM  = CurrentUsage (VRAM terpakai saat ini)
	// BudgetVRAM = Budget dari OS 
	UFUNCTION(BlueprintCallable, Category = "Benchmark|VRAM")
	static bool GetVRAMInfoMB(int32& TotalVRAM_MB, int32& UsedVRAM_MB, int32& BudgetVRAM_MB);
};
