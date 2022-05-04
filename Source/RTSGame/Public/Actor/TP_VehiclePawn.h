// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UnitBasePawn.h"
#include "WheeledVehicle.h"
#include "TP_VehiclePawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;

PRAGMA_DISABLE_DEPRECATION_WARNINGS

UCLASS(config=Game)
class ATP_VehiclePawn : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	ATP_VehiclePawn();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Component")
	UDecalComponent* SelectedComp;

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	FVector Target;

	float InitialDistance;

	float StartingTime;

	UPROPERTY()
	class ARTSGamePlayerController* RTSController;

	FTimerHandle UpdateMovementTimerHandle;
protected:
	virtual void BeginPlay() override;

public:

	void IsSelected(bool bIsSelected);

	void MoveToLocation(FVector TargetLocation);


	
	UFUNCTION()
	void UpdateMovement();
private:



public:

};

PRAGMA_ENABLE_DEPRECATION_WARNINGS

