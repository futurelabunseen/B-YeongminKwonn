// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacterNonPlayer.h"
#include "ACCharacterBoss.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACCharacterBoss : public AACCharacterNonPlayer
{
	GENERATED_BODY()

public:
	AACCharacterBoss();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void Tick(float DeltaTimes) override;
	void PlayLaserAttackAnimation();

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> WheelComponent;
	virtual void SetupHUDWidget(AACCharacterHUD* InHUDWidget) override;
	void SetStopRotation(bool InbStopRotation);

protected:
	virtual void BeginPlay() override;
	void LaserAttack();
	void UpdateLaserAttackRotate();

	UFUNCTION()
	void OnRep_ServerRotationYaw();
	virtual void SetDead() override;

protected:
	float FirstDelay = 5.f;
	float TotalRotation = 0.f;
	float AttackIterval = 0.02f;
	float RotateIterval = 0.05f;
	float LaserIterval = 0.5f;
	uint8 bisFiring : 1;
	FName AttachPoint = TEXT("spine_03Socket");

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UACBossAttackLaserComponent> AttackLaserComponent;
	
	FTimerHandle LaserAttackTimerHandle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> HeadComponent;

	UPROPERTY(Replicated)
	uint8 bStopRotation : 1;


	UPROPERTY(ReplicatedUsing = OnRep_ServerRotationYaw)
	float ServerRotationYaw;

	float RotationRate = 20.0f;
	float ClientTimeSinceUpdate = 0.0f;
	float ClientTimeBetweenLastUpdate = 0.0f;


};
