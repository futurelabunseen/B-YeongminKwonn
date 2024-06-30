// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyingMoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UBTTask_FlyingMoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FlyingMoveToTarget();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	float MaxSpeed = 500.f;
	float DefaultSpeed = 200.f;
	float VelocityAlpha = 0.25f;
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComponent;
	float Angle = 0.f;
	TObjectPtr<class APawn> ControllingPawn;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
