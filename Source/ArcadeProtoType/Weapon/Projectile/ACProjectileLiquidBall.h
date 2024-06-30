// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile/ACProjectileBase.h"
#include "ACProjectileLiquidBall.generated.h"


UENUM()
enum class ELiquidBallState : uint8
{
	FiredState,
	StuckState,
};

UCLASS()
class ARCADEPROTOTYPE_API AACProjectileLiquidBall : public AACProjectileBase
{
	GENERATED_BODY()

public:
	AACProjectileLiquidBall();
	virtual void LaunchToLocation(const FVector& StartLocation, const FVector& TargetLocation) override;
	
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent*  OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void SetSimulatePhysics(bool bSet);
	void ScaleUp(FVector OtherActorScale);

	FORCEINLINE ELiquidBallState GetLiquidBallState() const { return CurrentLiquidBallState; }
	bool IsTarget() const;
	class AACCharacterNonPlayer* GetTarget() const;

protected:
	virtual void BeginPlay() override;
	virtual void ResetProjectileMovement();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<AACCharacterNonPlayer> NPCTarget;

	UPROPERTY(EditAnywhere, Category = "StuckMaterial")
	TObjectPtr<UMaterialInstance> StuckMaterial;

	UPROPERTY(EditAnywhere, Category = "NiagaraComponent")
	TObjectPtr<class UNiagaraComponent> LiquidTraceEffect;

	UFUNCTION()
	void OnRep_UpdateLiquidState();

	UPROPERTY(ReplicatedUsing = OnRep_UpdateLiquidState)
	ELiquidBallState CurrentLiquidBallState;
	
	UPROPERTY(Replicated)
	uint8 bIsTarget : 1;
};
