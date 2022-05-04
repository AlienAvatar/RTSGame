// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/SelectionInterface.h"


// Add default functionality here for any ISelectionInterface functions that are not pure virtual.
void ISelectionInterface::I_SelectionUnit()
{
	//默认实现
	UE_LOG(LogTemp,Warning,TEXT("ISelectionInterface::I_SelectionUnit"));
}
