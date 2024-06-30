// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACMonsterAttackComponent.h"
#include "ACMinionAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADEPROTOTYPE_API UACMinionAttackComponent : public UACMonsterAttackComponent
{
	GENERATED_BODY()

public:	
	UACMinionAttackComponent();

public:
    virtual void InitializeComponent() override;
    void ActivateAttack(bool InBeginAttack);


protected:
    bool IsAttackEnable(const FVector TargetLocation);
    virtual void OnAttack() override;
    virtual void BeginAttack() override;
    virtual void EndAttack() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
    virtual void SpawnAttackEffect();

private:
    UPROPERTY(EditAnywhere, Category = "Attack")
    float HalfAttackAngle;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackInterval;

private:
    UPROPERTY(EditAnywhere, Category = "NiagaraSystem")
    TObjectPtr<class UNiagaraSystem> AttackEffectNS;

    UPROPERTY(EditAnywhere, Category = "NiagaraComponent")
    TObjectPtr<class UNiagaraComponent> AttackEffectComponent;

    FTimerHandle AttackTimerHandle;

    void OnChangeAttackBegin();

    UPROPERTY(ReplicatedUsing = OnRep_BeginAttack)
    uint8 bIsActiveAttack : 1;

    UFUNCTION()
    void OnRep_BeginAttack();
};
