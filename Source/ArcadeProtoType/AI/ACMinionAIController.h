// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ACAI.h"
#include "ACMinionAIController.generated.h"

/**
 *
 */
UCLASS()
class ARCADEPROTOTYPE_API AACMinionAIController : public AAIController
{
	GENERATED_BODY()
public:
	AACMinionAIController();

	void RunAI();
	void StopAI();

	class AACCharacterPlayer* GetTarget() const;

protected:
	void OnPossess(APawn* InPawn) override;
	void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AIPerception", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	// TObjectPtr<class UAISenseConfig_Sight> SightConfig;

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	void ProcessSights();
	void ProcessHearing();

	UPROPERTY()
	TObjectPtr<class AACCharacterPlayer> Target;

	ENpcState MinionMode = ENpcState::Roaming;
};
