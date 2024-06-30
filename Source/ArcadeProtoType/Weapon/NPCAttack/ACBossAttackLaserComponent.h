// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACMonsterAttackComponent.h"
#include "ACBossAttackLaserComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARCADEPROTOTYPE_API UACBossAttackLaserComponent : public UACMonsterAttackComponent
{
	GENERATED_BODY()

public:	
	UACBossAttackLaserComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginAttack() override;
	virtual void EndAttack() override;
	virtual void OnAttack() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void SpawnLaserHitEffect(const FVector EndLocation);
	void UpdateLaserEffect();

protected:
	// FromDataTable
	// FACBossAttack
	// FACBossAttack FACBossAttackInfo
	FVector LaserStartOffset;
	float LaserEndOffset;
	float LaserWidth;
	float LaserHitWidth;
	float LaserAttackDelayRate;
	int MaxLaserAttackCount;
	float AttackLoopRate;
	// FromDataTable
	int CurrentLaserAttackCount = 0;

	uint8 bLaserSpawned;
	uint8 bLaserHitSpawned;

	UPROPERTY(Replicated)
	FVector StartLocation;
	UPROPERTY(ReplicatedUsing = OnRep_EndLocation)
	FVector EndLocation;

	UPROPERTY(ReplicatedUsing = OnRep_bActiveLaser)
	uint8 bIsActiveLaser : 1;

	void UpdateEndLocation(FVector OUT& NewEnLocation);

	UFUNCTION()
	void OnRep_EndLocation();

	UFUNCTION()
	void OnRep_bActiveLaser();

	UFUNCTION()
	void SetActiveLaser(bool bActiveLaser);
	void OnChangeLaserActive();
	void OnChangeLaserEndLocation();
	void SpawnLaserEffect();

	void CheckHit();
	
	void SetStartLocation();

private:
	FTimerHandle AttackTimer;

	// Effect
	UPROPERTY(EditAnywhere, Category = "LaserParticle")
	TObjectPtr<class UParticleSystem> LaserParticleEffect;
	UPROPERTY(EditAnywhere, Category = "ParticleSystemComponent")
	TObjectPtr<class UParticleSystemComponent> LaserParticleComponent;

	UPROPERTY(EditAnywhere, Category = "LaserHitParticle")
	TObjectPtr<class UParticleSystem> LaserHitParticleEffect;
	UPROPERTY(EditAnywhere, Category = "ParticleSystemComponent")
	TObjectPtr<class UParticleSystemComponent> LaserHitParticleComponent;
};
