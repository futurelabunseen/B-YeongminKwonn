// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ACCharacterHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UACCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCADEPROTOTYPE_API IACCharacterHUDInterface
{
	GENERATED_BODY()

public:
	virtual void SetupHUDWidget(class AACCharacterHUD* InHUDWidget) = 0;
};
