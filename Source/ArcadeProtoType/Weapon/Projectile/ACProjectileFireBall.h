#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile/ACProjectileBase.h"
#include "ACProjectileFireBall.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACProjectileFireBall : public AACProjectileBase
{
	GENERATED_BODY()
public:
	AACProjectileFireBall();
	virtual void LaunchToLocation(const FVector& StartLocation, const FVector& TargetLocation) override;
	virtual void Launch(FVector ProjectileDirection);
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
	
UFUNCTION(NetMulticast, Reliable)
		void MulticastSpawnNiagaraEffect();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCActionCoopAttack(FVector Scale);
	UFUNCTION(Server, Reliable)
	void ServerRPCActionCoopAttack(FVector Scale);

	void ActionCoopAttack(FVector Scale);

private:
	UPROPERTY(EditAnywhere, Category = "StuckMaterial")
	TObjectPtr<UMaterialInstance> StuckMaterial;

	UPROPERTY(EditAnywhere, Category = "NiagaraComponent")
	TObjectPtr<class UNiagaraComponent> FireBallTraceEffect;

	UPROPERTY(EditAnywhere, Category = "ParticleSystem")
	TObjectPtr<class UParticleSystem> ExplosionParticleEffect;
};
