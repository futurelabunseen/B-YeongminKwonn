// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// #include "Interface/ABCharacterWidgetInterface.h"
// #include "Interface/ABCharacterItemInterface.h"
#include "Data/ACDataUtility.h"
#include "ACCharacterBase.generated.h"



UCLASS()
class ARCADEPROTOTYPE_API AACCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AACCharacterBase();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE ECharacterType GetCharacterType() const { return CharacterType; }
	FORCEINLINE bool IsDead() const { return bIsDead; }
protected:
	virtual void Tick(float DeltaSeconds);
	
protected:
	UPROPERTY(Replicated)
	TObjectPtr<class UACCharacterStatComponent> Stat;
	ECharacterType CharacterType;

	// Dead Section
protected:
	void PlayDeadAnimation();
	virtual void SetDead();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayDeadAnimation();

	UFUNCTION(Server, Unreliable)
	void ServerRPCPlayDeadAnimation();

protected:
	UPROPERTY()
	TObjectPtr<UMaterialInstance> DeadMaterialInstance;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DeadMaterialInstanceDynamic;
	virtual void UpdateMaterialDissolve(float DeltaTime);
	float DissolveValue = 1.f;
	virtual void OnChangeDeadState();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, ReplicatedUsing = OnRep_Dead)
	uint8 bIsDead : 1;

	UFUNCTION()
	void OnRep_Dead();
	virtual void ResetMaterial();
};
	
