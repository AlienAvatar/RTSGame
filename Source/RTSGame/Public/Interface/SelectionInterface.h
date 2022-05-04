// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USelectionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTSGAME_API ISelectionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void I_SelectionUnit() = 0;

	UFUNCTION()
	virtual void OnInputStart() = 0;

	UFUNCTION()
	virtual void OnInputHold() = 0;

	UFUNCTION()
	virtual void OnInputRelease() = 0;

	UFUNCTION()
	virtual void I_OnRightMouseClick() = 0;
};
