// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ACAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACAIController : public AAIController
{
	GENERATED_BODY()
public:
	AACAIController();

	void RunAI();
	void StopAI();

protected:
	void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
