// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ACDataUtility.h"
#include "ACCoopAttack.generated.h"

UCLASS()
class ARCADEPROTOTYPE_API AACCoopAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	AACCoopAttack();
	void SetScale(FVector NewScale);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
private:	
	FVector Scale;
	EProjectileType ProjectileType;

	UPROPERTY(EditAnywhere, Category = "ParticleComponent")
	TObjectPtr<class UParticleSystemComponent> ExplosionEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> SphereComponent;
	UPROPERTY(VisibleAnywhere, Category = "ProjectileMesh")
	TObjectPtr<UStaticMeshComponent> AttackMesh;
	UPROPERTY(EditAnywhere, Category = "ParticleSystem")
	TObjectPtr<class UParticleSystem> ParticleSystem;
};
