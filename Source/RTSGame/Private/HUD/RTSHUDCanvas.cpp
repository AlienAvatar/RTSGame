// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RTSHUDCanvas.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Controller/RTSGamePlayerController.h"
#include "Kismet/GameplayStatics.h"

ARTSHUDCanvas::ARTSHUDCanvas() :
	bIsDrawing(false)
{
}

void ARTSHUDCanvas::BeginPlay()
{
	Super::BeginPlay();
	RTSController = Cast<ARTSGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	
}

void ARTSHUDCanvas::DrawMarquee()
{
	DrawRect(RectColor,ClickedLocation.X,ClickedLocation.Y,HoldingLocation.X - ClickedLocation.X,HoldingLocation.Y - ClickedLocation.Y);
}



void ARTSHUDCanvas::I_SelectionUnit()
{
	ISelectionInterface::I_SelectionUnit();
	UE_LOG(LogTemp,Warning,TEXT("I_SelectionUnit_Implementation"));

}

void ARTSHUDCanvas::OnInputStart()
{
	ClickedLocation = RTSController->GetMouse2DPos();
	bIsDrawing = true;
}


void ARTSHUDCanvas::DrawHUD()
{
	Super::DrawHUD();

	if(bIsDrawing)
	{
		RTSController->ClearSelectedCharacter();
		SelectionCharArr.Empty();
		for(auto i : SelectionCharArr)
		{
			i->IsSelected(false);
		}
		HoldingLocation = RTSController->GetMouse2DPos();
		UE_LOG(LogTemp,Warning,TEXT("ClickedLocation X [%f] ClickedLocation Y [%f]"),ClickedLocation.X,ClickedLocation.Y);
		UE_LOG(LogTemp,Warning,TEXT("HoldingLocation X [%f] HoldingLocation Y [%f]"),HoldingLocation.X,HoldingLocation.Y);
		
		
		const float VectorLen = (HoldingLocation - ClickedLocation).Size();
		if(VectorLen > 10)
		{
			DrawRect(RectColor,ClickedLocation.X,ClickedLocation.Y,HoldingLocation.X - ClickedLocation.X,HoldingLocation.Y - ClickedLocation.Y);
			GetActorsInSelectionRectangle<ARTSGameCharacter>(
				ClickedLocation,
				HoldingLocation,
				SelectionCharArr,
				false,
				false);
		}
	}
}

void ARTSHUDCanvas::OnInputRelease()
{
	bIsDrawing = false;
	if(SelectionCharArr.Num() > 0)
	{
		for(auto i : SelectionCharArr)
		{
			RTSController->AddSelectedChar(i);
		}
	}
}

void ARTSHUDCanvas::OnInputHold()
{
}

void ARTSHUDCanvas::I_OnRightMouseClick()
{
	if(SelectionCharArr.Num() > 0)
	{
		for(int32 i = 0; i < SelectionCharArr.Num(); ++i)
		{
			FHitResult HitResult;
			RTSController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult);
			FVector MoveToLocation = HitResult.Location + FVector(i / 2 * 100, i % 2 * 100,0);
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(SelectionCharArr[i]->GetController(),MoveToLocation);
		}
	}
}
