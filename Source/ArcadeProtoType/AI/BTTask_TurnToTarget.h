// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ACAI.h"
#include "BTTask_TurnToTarget.generated.h"

/**
 * 
 */


UCLASS()
class ARCADEPROTOTYPE_API UBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_TurnToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(Category = Input, EditAnywhere)
	ENpcState NpcState;
};
