// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/ACCharacterPlayer.h"
#include "ACAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(OnEndDashDelegate);
DECLARE_MULTICAST_DELEGATE(OnEndDeadDelegate);

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UACAnimInstance();
	void PlayDashMontage();
	void PlayFireMontage();

	void PlayDefaultAim();
	void PlayAimMotion();

	void PlayDeadMontage();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	virtual void CheckHainging();

public:
	// FORCEINLINE void SetIsAiming(bool IsAiming) { bIsAiming = IsAiming; }
	void SetControlType(ECharacterControlType InControlType);
	OnEndDashDelegate OnEndDashEvent;
	OnEndDeadDelegate OnEndDeadEvent;

	UFUNCTION()
	void AnimNotify_EndDash();

	UFUNCTION()
	void AnimNotify_EndDead();

protected:
	ECharacterControlType CurrentControlType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DashAnimMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Fire, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> FireAnimMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Dead, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float AirSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bShouldMove : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsHanging : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	// uint8 bIsAiming : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aimoffset)
	FRotator AimOffsetRotator;
};
