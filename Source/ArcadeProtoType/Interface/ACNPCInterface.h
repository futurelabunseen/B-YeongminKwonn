// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
// C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\AIModule\Classes\BehaviorTree\BehaviorTreeComponent.h
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "ACNPCInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UACNPCInterface : public UInterface
{
	GENERATED_BODY()
};
DECLARE_DELEGATE(FNPCAttackFinished);
DECLARE_MULTICAST_DELEGATE_TwoParams(FNPCFlyingMoveFinished, UBehaviorTreeComponent& , EBTNodeResult::Type);
// UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type TaskResult
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIFlyingMoveFinish, UBehaviorTreeComponent*, InAcotr, AActor*, InAcotr2);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIFlyingMoveFinish, (UBehaviorTreeComponent*), OwnerComp, EBTNodeResult::Type, TaskResult);
/**
 * 
 */
class ARCADEPROTOTYPE_API IACNPCInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIAttackRange() = 0;
	virtual float GetDetectRadius() = 0;
	
	virtual void SetAIAttackDelegate(const FNPCAttackFinished& InOnAttackFinished) = 0;
	virtual void AttackByAI() = 0;
	virtual float GetAITurnSpeed() = 0;
	virtual float GetDestinationTolerance_InRoaming() = 0;
	virtual float GetDestinationTolerance_InPatrol() = 0;
	FNPCFlyingMoveFinished OnFlyingMoveFinished;
	virtual APawn* GetTarget() const = 0;
};
