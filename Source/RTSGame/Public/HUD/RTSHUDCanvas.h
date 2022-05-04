// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/RTSGameCharacter.h"
#include "GameFramework/HUD.h"
#include "Interface/SelectionInterface.h"
#include "RTSHUDCanvas.generated.h"

/**
 * 
 */
UCLASS()
class RTSGAME_API ARTSHUDCanvas : public AHUD,public ISelectionInterface
{
	GENERATED_BODY()

public:
	ARTSHUDCanvas();
	
protected:
	FVector2D ClickedLocation;
	
	FVector2D HoldingLocation;
	
	FVector2D CurrentPoint;

	bool bIsDrawing;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="References")
	class ARTSGamePlayerController* RTSController;

	UPROPERTY(EditAnywhere)
	TArray<ARTSGameCharacter*> SelectionCharArr;

	UPROPERTY(EditDefaultsOnly,Category="HUD")
	FLinearColor RectColor;
protected:
	virtual void BeginPlay() override;

	void DrawMarquee();
	
public:
	virtual void I_SelectionUnit() override;

	virtual void OnInputStart() override;
	
	virtual void DrawHUD() override;

	virtual void OnInputRelease() override;

	virtual void OnInputHold() override;

	virtual void I_OnRightMouseClick() override;
};
