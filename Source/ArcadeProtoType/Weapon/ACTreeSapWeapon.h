// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ACWeaponBase.h"
#include "ACTreeSapWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACTreeSapWeapon : public AACWeaponBase
{
	GENERATED_BODY()

public:
	AACTreeSapWeapon();
	
public:
	virtual bool OnFire(bool bIsDead = false) override;
	FORCEINLINE float GetShootInterval()const { return ShootInterval; }

protected:
	virtual void PostInitializeComponents() override;
	virtual void CreateProjectile(FVector StartLocation, FRotator TargetRotation) override;
	
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void SeverRPCCreateProjectile(FVector StartLocation, FRotator TargetRotation);

private:
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AACProjectileLiquidBall> LiquidProjectileClass;
	float ShootInterval = 0.1f;
};
