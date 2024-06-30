// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/ACDataUtility.h"
#include "ACAISpawner.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UACAISpawner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */


class ARCADEPROTOTYPE_API IACAISpawner
{
	GENERATED_BODY()
public:
	virtual void SpawnMonster(EMonsterType MonsterType) = 0;
};
