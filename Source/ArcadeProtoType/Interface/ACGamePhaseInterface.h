// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Game/GamePhase/ACGamePhase.h"
#include "ACGamePhaseInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChangedDelegate, EACGamePhaseList);

UINTERFACE(MinimalAPI)
class UACGamePhaseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCADEPROTOTYPE_API IACGamePhaseInterface
{
	GENERATED_BODY()

public:
	virtual void SetGamePhase(EACGamePhaseList NewPhase) = 0;
	FOnGamePhaseChangedDelegate OnChangedGamePhase;
};
