// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ACBossAnimInstance.h"

UACBossAnimInstance::UACBossAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAnimMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Arcade/Animation/AnimMontage/AM_MonsterLaserAttak.AM_MonsterLaserAttak'"));
	if (AttackAnimMontageRef.Object)
	{
		LaserAttackAnimMontage = AttackAnimMontageRef.Object;
	}
}

void UACBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
}

void UACBossAnimInstance::PlayLaserAttackMontage()
{
	if (!Montage_IsPlaying(LaserAttackAnimMontage))
	{
		Montage_Play(LaserAttackAnimMontage, 1.0f);
	}
}

void UACBossAnimInstance::AnimNotify_EndLaserAttackAnimation()
{
	if (OnEndLaserAttackAnimationEvent.IsBound()) 
	{
		OnEndLaserAttackAnimationEvent.Broadcast();
	}
}
