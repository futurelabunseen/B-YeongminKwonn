// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ACMonsterAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UACMonsterAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCADEPROTOTYPE_API IACMonsterAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginAttack() = 0;
	virtual void OnAttack() = 0;
	virtual void EndAttack() = 0;
	virtual void ApplyDamage(APawn* Target) = 0;
};
