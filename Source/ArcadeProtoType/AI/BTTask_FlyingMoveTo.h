// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ACAI.h"
#include "BTTask_FlyingMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UBTTask_FlyingMoveTo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FlyingMoveTo();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(Category = Input, EditAnywhere)
	ENpcState NpcState = ENpcState::Roaming;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


protected:
	UPROPERTY(Category = Input, EditAnywhere)
	float VelocityUpdateDeltaTime = 0.2f;

	UPROPERTY(Category = Input, EditAnywhere)
	uint8 bIsFaceTarget : 1;


protected:
	TObjectPtr<class APawn> ControllingPawn;
	float Angle = 0.f;
	float DefaultSpeed = 800.f;
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComponent;
};
