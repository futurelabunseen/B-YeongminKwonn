
#pragma once

#include "CoreMinimal.h"
#include "Weapon/ACWeaponBase.h"
#include "ACMatchStickWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACMatchStickWeapon : public AACWeaponBase
{
	GENERATED_BODY()

public:
	AACMatchStickWeapon();
	
public:
	virtual bool OnFire(bool bIsDead = false) override;
	bool IsDetecting();

protected:
	virtual void PostInitializeComponents() override;
	virtual void CreateProjectile(FVector StartLocation, FRotator TargetRotation) override;
	
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void SeverRPCCreateProjectile(FVector StartLocation, FRotator TargetRotation);

private:
	UPROPERTY(EditAnywhere , Category = Projectile)
	TSubclassOf<class AACProjectileFireBall> FireBallProjectileClass;
	uint8 bIsTarget : 1;
};
