// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ACDataUtility.h"
#include "ACProjectileBase.generated.h"

UCLASS()
class ARCADEPROTOTYPE_API AACProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AACProjectileBase();
	UFUNCTION()
	virtual void LaunchToLocation(const FVector& StartLocation, const FVector& TargetLocation);
	UFUNCTION()
	void SetProjectileSpeed(float INProjectileSpeed);

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	EProjectileType ProjectileType;

protected:
	UPROPERTY(VisibleAnywhere, Category = "ProjectileMovementComponent")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
	UPROPERTY(Replicated, VisibleAnywhere, Category = "SphereComponent")
	TObjectPtr<class USphereComponent> SphereComponent;

	UPROPERTY(Replicated, EditAnywhere, Category = "ProjectileMesh")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "NiagaraSystem")
	TObjectPtr<class UNiagaraSystem> NaiagaraSystem;
};
