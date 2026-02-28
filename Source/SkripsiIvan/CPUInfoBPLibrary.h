// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CPUInfoBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SKRIPSIIVAN_API UCPUInfoBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Nama CPU (sekali ambil untuk UI)
	UFUNCTION(BlueprintPure, Category="Benchmark|CPU")
	static FString GetCPUName();

	// Reset sampler (panggil saat benchmark begin supaya baseline bersih)
	UFUNCTION(BlueprintCallable, Category="Benchmark|CPU")
	static void ResetProcessCPUUsageSampler();

	// CPU usage % milik proses game ini (Windows). Return false jika belum ada sample sebelumnya.
	UFUNCTION(BlueprintCallable, Category="Benchmark|CPU")
	static bool GetProcessCPUUsagePercent(float& OutCPUPercent);

};
