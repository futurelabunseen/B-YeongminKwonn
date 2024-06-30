// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ACDataUtility.h"
#include "ACWeaponBase.generated.h"

DECLARE_DELEGATE(FOnZeroAmmoDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedDelegate, int /*CurrentHp*/, int /*MaxHp*/);
UCLASS()
class ARCADEPROTOTYPE_API AACWeaponBase : public AActor// , public IACCharacterSetterInterface
{
	GENERATED_BODY()
	
public:	
	AACWeaponBase();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE virtual void SetMaxAmmoCount(int32 InMaxAmmoCount) { MaxAmmoCount = InMaxAmmoCount; }
	FORCEINLINE virtual void SetMaxShootRange(float InMaxShootRange) { MaxShootRange = InMaxShootRange; }

	FORCEINLINE virtual int32 GetAmmoCount() const { return CurrentAmmoCount; }
	FORCEINLINE virtual bool HasAmmo() const { return CurrentAmmoCount > 0; }

	FORCEINLINE virtual FVector GetMuzzleLocation() const { return WeaponMesh->GetSocketLocation(FName(TEXT("Muzzle"))); };
	FORCEINLINE virtual FTransform GetMuzzleTransform() const { return WeaponMesh->GetSocketTransform(FName(TEXT("Muzzle"))); };
	FORCEINLINE virtual FRotator GetMuzzleRotation() const { return WeaponMesh->GetSocketRotation(FName(TEXT("Muzzle"))); };
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	// Interface
	virtual void Equip();
	virtual bool OnFire(bool bIsDead = false);
	virtual void EndFire();
	FORCEINLINE int GetCurrentAmmo() const { return CurrentAmmoCount; }
	FORCEINLINE int GetMaxAmmo() const { return  MaxAmmoCount; }

	void ResetWeapon();// { CurrentAmmoCount = MaxAmmoCount; }
	virtual void SetActorHiddenInGame(bool) override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

protected:
	void ReloadAmmo();
	void ReloadAmmoOnTimer();
	FTimerHandle ReloadAmmoTimer;
	// Interface
	virtual void CreateProjectile(FVector StartLocation, FRotator TargetRotation);

public:
	FOnAmmoChangedDelegate OnAmmoChanged;

protected:
	FOnZeroAmmoDelegate OnAmmoZero;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "WeaponSkeletalMesh")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	UPROPERTY(Replicated)
	TObjectPtr<class AACCharacterPlayer> Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FVector WeaponRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FRotator WeaponRotator;

protected:
	UPROPERTY(Replicated)
	int32 CurrentAmmoCount;

	void SetAmmo(int NewAmmoCount);
	
	// from Data Asset
	int32 MaxAmmoCount;
	float MaxShootRange;
	EWeaponType WeaponType;
};
