// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ACDataUtility.h"
#include "HUD/ACCharacterHUD.h"
#include "ACShootingComponent.generated.h"

// DECLARE_MULTICAST_DELEGATE(FOnAimingDelegate);
// DECLARE_MULTICAST_DELEGATE_OneParam(FOnDetectedDelegate, bool/*IsDetecting*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAimStatusDelegate, EAimType/*AimType*/);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADEPROTOTYPE_API UACShootingComponent : public UActorComponent // , public IACCharacterSetterInterface
{
	GENERATED_BODY()

public:	
	UACShootingComponent();

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void SpawnWeapon();
	
	virtual int GetCurrentAmmo() const;
	virtual int GetMaxAmmo() const;
	virtual void ResetWeapon();
	virtual void SetWeaponHiddenInGame(bool bIsHidden);
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// virtual void SetCharacter(class AACCharacterBase* InCharacter) override;

	virtual void SetZoomFOV(float DeltaTime);

	virtual void OnFire();
	virtual void EndFire();

	// FOnDetectedDelegate OnDetectedEvent;
	FOnAimStatusDelegate OnAimStatusChanged;
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void OnDetecting();

private:
	float CurrentFOV;
	float DefaultFOV =90.f;
	float AimFOV = 80.0f;
	float ZoomInterpSpeed = 25.0f;
	FTimerHandle AutoFireTimerHandle;

protected:
	TObjectPtr<class AACCharacterPlayer> Character;
	UPROPERTY(ReplicatedUsing=OnRep_Weapon)
	TObjectPtr<class AACWeaponBase> Weapon;

	UFUNCTION()
	void OnRep_Weapon();
};
