// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACGamePhase.h"
#include "ACFinalCombatPhase.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACFinalCombatPhase : public AACGamePhase
{
	GENERATED_BODY()
	

public:
	AACFinalCombatPhase();
	virtual void OnBeginPhase()override;
	virtual void OnPlayPhase(float DeltaTime)override;
	virtual void OnEndPhase()override;

protected:
	virtual void StartSpawnMosterAfterDelay();
	virtual void StartSpawnPlayerAfterDelay();

protected:
	FTimerHandle MonsterDelayTimerHandle;
	FTimerHandle PlayerDelayTimerHandle;
};
