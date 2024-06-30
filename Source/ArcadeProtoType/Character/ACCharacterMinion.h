// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacterNonPlayer.h"
#include "ACCharacterMinion.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACCharacterMinion : public AACCharacterNonPlayer// , public IACNPCInterface
{
	GENERATED_BODY()

public:
	AACCharacterMinion();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void BeginAttack();
	virtual void EndAttack();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetDead() override;

public:
	virtual float GetAIAttackRange() override;
	virtual void SetAIAttackDelegate(const FNPCAttackFinished& InOnAttackFinished) override;
	virtual float GetDetectRadius() override;
	virtual float GetAITurnSpeed() override;
	virtual float GetDestinationTolerance_InRoaming() override;
	virtual float GetDestinationTolerance_InPatrol() override;
	virtual void AttackByAI() override;

protected:
	FNPCAttackFinished OnAttackFinished;
	TObjectPtr<class UACMinionAttackComponent> MinionAttackComponent;

};
