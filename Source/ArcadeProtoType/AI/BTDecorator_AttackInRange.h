// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackInRange.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UBTDecorator_AttackInRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_AttackInRange();
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
