// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ACAI.h"
#include "BTTask_FindLocation.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UBTTask_FindLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindLocation();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(Category = Input, EditAnywhere)
	float MinXYDistanceFromTargetActor = 100.f;
	
	UPROPERTY(Category = Input, EditAnywhere)
	float MaxXYDistanceFromTargetActor = 500.f;
	
	UPROPERTY(Category = Input, EditAnywhere)
	float ExtraAddedElevationToFinalLocation = 20.f;
	
	UPROPERTY(Category = Input, EditAnywhere)
	ENpcState NpcState = ENpcState::Roaming;

public:
	FVector GetRandomPatrolPoint();
	
};
