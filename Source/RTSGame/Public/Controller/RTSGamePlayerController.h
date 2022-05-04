// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/RTSGameCharacter.h"
#include "Actor/TP_VehiclePawn.h"
#include "GameFramework/PlayerController.h"
#include "HUD/RTSHUDCanvas.h"
#include "RTSGamePlayerController.generated.h"

UCLASS()
class ARTSGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARTSGamePlayerController();

protected:
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<ARTSGameCharacter> RTSGameCharacterClass;

	UPROPERTY()
	class ARTSHUDCanvas* HUDCanvas;
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void Test();

	TArray<ARTSGameCharacter*> SelectedCharArr;
	
	TArray<ATP_VehiclePawn*> SelectedVehicleArr;

	void MouseLeftPressed();

	void MouseLeftReleased();

	void MouseRightPressed();


public:
	void AddSelectedVehicle(ATP_VehiclePawn* VehiclePawn);

	void RemoveSelectedVehicle(ATP_VehiclePawn* VehiclePawn);

	void ClearSelectedVehicle();

	void AddSelectedChar(ARTSGameCharacter* Char);

	void RemoveSelectedCharacter(ARTSGameCharacter* Char);

	void ClearSelectedCharacter();
	
	FVector2D GetMouse2DPos();
};








