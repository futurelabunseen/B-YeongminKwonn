// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Game/GamePhase/ACGamePhase.h"
#include "ACGameModePhaseInterface.generated.h"




UINTERFACE(MinimalAPI)
class UACGameModePhaseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCADEPROTOTYPE_API IACGameModePhaseInterface
{
	GENERATED_BODY()

public:
	virtual void SetGamePhase(EACGamePhaseList NewPhase) = 0;

};
