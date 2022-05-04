// Fill out your copyright notice in the Description page of Project Settings.


#include "Lib/RTSBlueprintFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

FVector URTSBlueprintFunctionLibrary::SetCursorWorldPlacement(const UObject* WorldContextObject,ARTSGamePlayerController* Controller, float SightDistance)
{
	FVector Result(0,0,0);
	FVector WorldLocation;
	FVector WorldDirection;
	Controller->DeprojectMousePositionToWorld(WorldLocation,WorldDirection);

	const FVector StartVector = WorldLocation;
	const FVector EndVector = WorldDirection * SightDistance + WorldLocation;
	FHitResult HitResult;
	//GetWorld()在静态方法中无法调用
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	World->LineTraceSingleByChannel(HitResult,StartVector,EndVector,ECollisionChannel::ECC_Visibility);
	if(HitResult.Actor != nullptr)
	{
		Result = HitResult.Location;
	}
	
	return Result;
}
