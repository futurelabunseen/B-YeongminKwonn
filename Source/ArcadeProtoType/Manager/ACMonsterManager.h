// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Game/GamePhase/ACGamePhase.h"
#include "Interface/ACCharacterHUDInterface.h"
#include "ACMonsterManager.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnDeadMonsterDelegate);
/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACMonsterManager : public UWorldSubsystem, public IACCharacterHUDInterface
{
	GENERATED_BODY()
public:
	UACMonsterManager();
	void AddMinionMonster(class AACCharacterMinion* Minion);
	void SetBossMonster(class AACCharacterBoss* Boss);
	void DestroyBossMonster();
	void SetupHUDWidget(class AACCharacterHUD* InHUDWidget);
	FORCEINLINE int32 GetMonsterNum()const { return MinionArray.Num(); }
	FOnDeadMonsterDelegate OnAllMonsterDead;
	virtual void RemoveMinionFromArray(class AActor* Minion);

protected:
	void Initialize(FSubsystemCollectionBase& Collection);
	TArray<class AACCharacterMinion*> MinionArray;
	TObjectPtr<class AACCharacterBoss> MonsterBoss;

	TMap<EACGamePhaseList, int32> MaxMinionNum;
	TMap<EACGamePhaseList, int32> SpawnedMinionNum;
	uint8 bIsAllMinionSpawned : 1;
};
