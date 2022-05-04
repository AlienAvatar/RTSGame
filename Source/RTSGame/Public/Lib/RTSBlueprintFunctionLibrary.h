// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Controller/RTSGamePlayerController.h"
#include "RTSBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RTSGAME_API URTSBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//获取FVector的几何坐标
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FVector SetCursorWorldPlacement(const UObject* WorldContextObject,ARTSGamePlayerController* Controller,float SightDistance);
};
