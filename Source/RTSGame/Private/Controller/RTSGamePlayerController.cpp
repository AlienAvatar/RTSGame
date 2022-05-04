// Copyright Epic Games, Inc. All Rights Reserved.

#include "Controller/RTSGamePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "K2Node_SpawnActorFromClass.h"
#include "Actor/RTSGameCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/World.h"
#include "GameFramework/HUD.h"
#include "Interface/SelectionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Lib/RTSBlueprintFunctionLibrary.h"

ARTSGamePlayerController::ARTSGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ARTSGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDCanvas = Cast<ARTSHUDCanvas>(GetHUD());
}

void ARTSGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ARTSGamePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ARTSGamePlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARTSGamePlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ARTSGamePlayerController::MoveToTouchLocation);

	InputComponent->BindAction("Test",IE_Pressed, this, &ARTSGamePlayerController::Test);
	
	InputComponent->BindAction("MouseLeft",IE_Pressed, this, &ARTSGamePlayerController::MouseLeftPressed);
	InputComponent->BindAction("MouseLeft",IE_Released, this, &ARTSGamePlayerController::MouseLeftReleased);
	InputComponent->BindAction("MouseRight",IE_Pressed, this, &ARTSGamePlayerController::MouseRightPressed);
}


void ARTSGamePlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ARTSGameCharacter* MyPawn = Cast<ARTSGameCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ARTSGamePlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ARTSGamePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ARTSGamePlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ARTSGamePlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void ARTSGamePlayerController::Test()
{
	FVector Location = URTSBlueprintFunctionLibrary::SetCursorWorldPlacement(GetWorld(),this,1000);
	if(RTSGameCharacterClass)
	{
		GetWorld()->SpawnActor<ARTSGameCharacter>(RTSGameCharacterClass,Location,FRotator::ZeroRotator);
	}
}

void ARTSGamePlayerController::AddSelectedVehicle(ATP_VehiclePawn* VehiclePawn)
{
	SelectedVehicleArr.Add(VehiclePawn);
	VehiclePawn->IsSelected(true);
}

void ARTSGamePlayerController::RemoveSelectedVehicle(ATP_VehiclePawn* VehiclePawn)
{
	SelectedVehicleArr.Remove(VehiclePawn);
	VehiclePawn->IsSelected(false);
}

void ARTSGamePlayerController::ClearSelectedVehicle()
{
	for(auto i : SelectedVehicleArr)
	{
		i->IsSelected(false);
	}
	SelectedVehicleArr.Empty();
}

void ARTSGamePlayerController::AddSelectedChar(ARTSGameCharacter* Char)
{
	SelectedCharArr.Add(Char);
	Char->IsSelected(true);
}

void ARTSGamePlayerController::RemoveSelectedCharacter(ARTSGameCharacter* Char)
{
	SelectedCharArr.Remove(Char);
	Char->IsSelected(false);
}

void ARTSGamePlayerController::ClearSelectedCharacter()
{
	for(auto i : SelectedCharArr)
	{
		i->IsSelected(false);
	}
	SelectedCharArr.Empty();
}

FVector2D ARTSGamePlayerController::GetMouse2DPos()
{
	float PosX;
	float PosY;
	GetMousePosition(PosX,PosY);
	return FVector2D(PosX,PosY);
}

void ARTSGamePlayerController::MouseLeftPressed()
{
	//第一种方法调用接口
	/*if(GetHUD()->GetClass()->ImplementsInterface(USelectionInterface::StaticClass()))
	{
		Cast<ISelectionInterface>(GetHUD())->I_SelectionUnit();
	}*/
	//第二种方法调用接口
	ISelectionInterface* SelectionInterface = Cast<ISelectionInterface>(GetHUD());//For Calling only C++
	if(SelectionInterface)
	{
		SelectionInterface->OnInputStart();
	}
}

void ARTSGamePlayerController::MouseLeftReleased()
{
	ISelectionInterface* SelectionInterface = Cast<ISelectionInterface>(GetHUD());//For Calling only C++
	if(SelectionInterface)
	{
		SelectionInterface->OnInputRelease();
	}
}

void ARTSGamePlayerController::MouseRightPressed()
{
	ISelectionInterface* SelectionInterface = Cast<ISelectionInterface>(GetHUD());//For Calling only C++
	if(SelectionInterface)
	{
		SelectionInterface->I_OnRightMouseClick();
	}
}
