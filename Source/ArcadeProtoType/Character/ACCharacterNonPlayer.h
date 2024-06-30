// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacterBase.h"
#include "Interface/ACCharacterHUDInterface.h"
#include "Interface/ACNPCInterface.h"
#include "ACCharacterNonPlayer.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class MonsterAIState : uint8
{
	Patrol UMETA(DisplayName = "Patrol"),
	Chase UMETA(DisplayName = "Chase"),
	Fall UMETA(DisplayName = "Fall"),
	Investigate UMETA(DisplayName = "Investigate"),
};

UENUM(BlueprintType)
enum class ChaseState : uint8
{
	Chase UMETA(DisplayName = "Chase"),
	BeingHit UMETA(DisplayName = "BeingHit"),
	Explode UMETA(DisplayName = "Explode")
};


UCLASS()
class ARCADEPROTOTYPE_API AACCharacterNonPlayer : public AACCharacterBase, public IACCharacterHUDInterface , public IACNPCInterface
{
	GENERATED_BODY()
	
public:
	AACCharacterNonPlayer();

public:
	void SetupHUDWidget(AACCharacterHUD* InHUDWidget);

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetStuckProjectile() const;

	void AddStuckProjectileArray(AActor* StuckProjectile);

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> StuckProjectileArray;

	FORCEINLINE bool IsTargetChanged()const { return bIsTargetChanged; }
	FORCEINLINE void SetTargetChanged(bool NewbIsTargetChanged) { bIsTargetChanged = NewbIsTargetChanged; }
	FORCEINLINE EMonsterType GetMonsterType() const {return MonsterType; }


protected:
	virtual void SetDead() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<class AACPlayerController*> TargetArray;
	
	EMonsterType MonsterType;

public:
	UFUNCTION(BlueprintCallable)
	virtual APawn* GetTarget() const;
	virtual APawn* GetAttackTarget();
	
protected:
	TObjectPtr <APawn> AttackTarget;
	int LastTargetIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsTargetChanged : 1;

public:
	UFUNCTION(BlueprintCallable)
	void ChangeTarget();

	void SetTarget(class AACCharacterPlayer* Player);
	TObjectPtr<class AACCharacterPlayer> Target;


public:
	virtual float GetAIAttackRange();
	virtual void SetAIAttackDelegate(const FNPCAttackFinished& InOnAttackFinished);
	virtual float GetDetectRadius();
	virtual float GetAITurnSpeed();
	virtual float GetDestinationTolerance_InRoaming();
	virtual float GetDestinationTolerance_InPatrol();
	virtual void AttackByAI();
};
