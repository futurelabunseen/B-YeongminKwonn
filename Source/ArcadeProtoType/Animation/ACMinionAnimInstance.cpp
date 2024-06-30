// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ACMinionAnimInstance.h"
#include "Character/ACCharacterMinion.h"

UACMinionAnimInstance::UACMinionAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAnimMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Arcade/Animation/AnimMontage/AM_Attack.AM_Attack'"));
	if (AttackAnimMontageRef.Object)
	{
		AttackAnimMontage = AttackAnimMontageRef.Object;
	}
}

void UACMinionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UACMinionAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackAnimMontage))
	{
		Montage_Play(AttackAnimMontage, 1.0f);
	}
}

void UACMinionAnimInstance::AnimNotify_EndAttack()
{
	if (OnEndAttackEvent.IsBound()) 
	{
		OnEndAttackEvent.Broadcast();
	}
}

void UACMinionAnimInstance::AnimNotify_StartAttack()
{
	if (OnStartAttackEvent.IsBound())
	{
		OnStartAttackEvent.Broadcast();
	}
}


