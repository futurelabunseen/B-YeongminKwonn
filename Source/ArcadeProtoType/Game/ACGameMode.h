// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/ACDataUtility.h"
#include "Interface/ACAISpawner.h"
#include "ACGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACGameMode : public AGameModeBase, public IACAISpawner
{
	GENERATED_BODY()
public:
	AACGameMode();
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	void SpawnMonster(EMonsterType MonsterType);

private:
	void SpawnBoss();
	void SpawnMinion();

private:
	TMap<ECharacterType, UClass*> CharacterTypes;
	ECharacterType SelectedCharacterType;

	TSubclassOf<class AACCharacterBoss> BossClass;
	TSubclassOf<class AACCharacterMinion> MinionClass;
	
	int count = 0;
};
