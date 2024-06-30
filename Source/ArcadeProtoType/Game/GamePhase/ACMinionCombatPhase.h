// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACGamePhase.h"
#include "ACMinionCombatPhase.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACMinionCombatPhase : public AACGamePhase
{
	GENERATED_BODY()
	
public:
	AACMinionCombatPhase();
	virtual void OnBeginPhase()override;
	virtual void OnPlayPhase(float DeltaTime)override;
	virtual void OnEndPhase()override;
	virtual void ChangeGamePhase();

protected:
	uint8 bStopCombat:1;
};
