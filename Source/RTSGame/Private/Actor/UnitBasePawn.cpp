// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/UnitBasePawn.h"

#include "Components/DecalComponent.h"

// Sets default values
AUnitBasePawn::AUnitBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SelectedComp = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectedComp"));
	SelectedComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUnitBasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnitBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUnitBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

