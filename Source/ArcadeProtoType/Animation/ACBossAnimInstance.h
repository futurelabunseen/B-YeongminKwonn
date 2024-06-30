// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ACMinionAnimInstance.h"
#include "ACBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UACBossAnimInstance();
	virtual void NativeInitializeAnimation() override;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> LaserAttackAnimMontage;

public:
	OnAttackDelegate OnEndLaserAttackAnimationEvent;
	// OnAttackDelegate OnStartAttackEvent;
	void PlayLaserAttackMontage();
	UFUNCTION()
	void AnimNotify_EndLaserAttackAnimation();
};
