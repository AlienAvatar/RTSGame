// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actor/TP_VehiclePawn.h"
#include "../../TP_Vehicle/TP_VehicleWheelFront.h"
#include "../../TP_Vehicle/TP_VehicleWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Components/DecalComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Controller/RTSGamePlayerController.h"
#include "Materials/Material.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


#ifndef HMD_MODULE_INCLUDED
#define HMD_MODULE_INCLUDED 0
#endif

// Needed for VR Headset
#if HMD_MODULE_INCLUDED
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#endif // HMD_MODULE_INCLUDED

#define LOCTEXT_NAMESPACE "VehiclePawn"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

ATP_VehiclePawn::ATP_VehiclePawn()
{
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	
	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UTP_VehicleWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UTP_VehicleWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UTP_VehicleWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UTP_VehicleWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	SelectedComp = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectedComp"));
	SelectedComp->SetupAttachment(RootComponent);
	SelectedComp->SetVisibility(false);
}

void ATP_VehiclePawn::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	UE_LOG(LogTemp,Warning,TEXT("Hello"));
	if(ButtonPressed.GetFName() == "LeftMouseButton")
	{
		IsSelected(true);
	}
	
}

void ATP_VehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	RTSController = Cast<ARTSGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
}

void ATP_VehiclePawn::IsSelected(bool bIsSelected)
{
	SelectedComp->SetVisibility(bIsSelected);
}

void ATP_VehiclePawn::MoveToLocation(FVector TargetLocation)
{
	Target = TargetLocation;
	//与目标的距离
	InitialDistance = ( GetActorLocation() - Target ).Size();
	StartingTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());

	GetVehicleMovement()->SetHandbrakeInput(false);
	GetWorld()->GetTimerManager().SetTimer(
		UpdateMovementTimerHandle,
		this,
		&ATP_VehiclePawn::UpdateMovement,
		0.01,
		true
	);
}

void ATP_VehiclePawn::UpdateMovement()
{
	const FVector CurrentLocation = GetActorLocation();
	const float CurrentDistance = (CurrentLocation - Target ).Size();
	
	const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation,Target);
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(LookRotation,GetActorRotation());
	//判断车辆的行驶方向
	const float RangeValue = UKismetMathLibrary::MapRangeClamped(DeltaRotation.Yaw,-180.f,180.f,-1,1);
	//true，前方行驶，false，后方行驶
	const bool bRange = UKismetMathLibrary::InRange_FloatFloat(DeltaRotation.Yaw,-90.f,90.f,true,true);
	//如果是前方行驶
	float SelectValue = UKismetMathLibrary::SelectFloat(RangeValue,0.f,bRange);
	float SelectValue2 = UKismetMathLibrary::SelectFloat(SelectValue,RangeValue,!bRange && CurrentDistance < 10000);

	const float SteeringValue = UKismetMathLibrary::SelectFloat(-1.f,SelectValue2,SelectValue == 0.f);
	GetVehicleMovement()->SetSteeringInput(SteeringValue);

	//设置
	float CurrentInput;


	float TempValue = CurrentDistance / InitialDistance * UKismetMathLibrary::MultiplyMultiply_FloatFloat(
		InitialDistance * 0.0000025 + 1,
		UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - InitialDistance);
	TempValue = UKismetMathLibrary::SelectFloat(
		TempValue * UKismetMathLibrary::SelectFloat(1,-1,bRange),
		TempValue * 0.5,
		CurrentDistance / InitialDistance < 0);
	TempValue = UKismetMathLibrary::Clamp(TempValue,-10,10);
	CurrentInput = TempValue;																												
	GetVehicleMovement()->SetThrottleInput(CurrentInput);
}

#undef LOCTEXT_NAMESPACE

PRAGMA_ENABLE_DEPRECATION_WARNINGS
