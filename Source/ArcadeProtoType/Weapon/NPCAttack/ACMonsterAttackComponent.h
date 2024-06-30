// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ACDataUtility.h"
#include "ACMonsterAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADEPROTOTYPE_API UACMonsterAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACMonsterAttackComponent();

public:
	virtual void BeginAttack();
	virtual void EndAttack();

protected:
	virtual void InitializeComponent() override;

protected:
	virtual void OnAttack();

	// Minion은 검증이 필요하다.
	virtual void ApplyDamage(APawn* Target);

protected:
	float AttackDamageAmount;
	FName AttachPoint;
	float AttackRange;
	// EMonsterType MonsterType;
};
