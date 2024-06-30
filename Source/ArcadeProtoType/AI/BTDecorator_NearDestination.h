// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_NearDestination.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UBTDecorator_NearDestination : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_NearDestination();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
