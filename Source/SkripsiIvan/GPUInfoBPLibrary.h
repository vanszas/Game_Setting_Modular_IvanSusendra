// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GPUInfoBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SKRIPSIIVAN_API UGPUInfoBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Benchmark|GPU")
	static FString GetGPUName();

	UFUNCTION(BlueprintCallable, Category = "Benchmark|GPU")
	static void ResetGPUProcessUsageSampler(bool bOnly3DEngine = true);

	UFUNCTION(BlueprintCallable, Category = "Benchmark|GPU")
	static bool GetGPUProcessUsagePercent(float& OutGPUPercent);

};
