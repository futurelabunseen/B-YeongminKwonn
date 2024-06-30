// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckFinalLocation.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UBTService_CheckFinalLocation : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckFinalLocation();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
