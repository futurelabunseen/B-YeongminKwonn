// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Interface/ACGamePhaseInterface.h"
#include "Interface/ACCharacterHUDInterface.h"
#include "ACGameState.generated.h"

UENUM()
enum class EACGameResult : uint8
{
	None,
	Victory UMETA(DisplayName = "Victory"),
	Defeat UMETA(DisplayName = "Defeat"),
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedGameResultDelegate, EACGameResult);

UCLASS()
class ARCADEPROTOTYPE_API AACGameState : public AGameState, public IACGamePhaseInterface, public IACCharacterHUDInterface
{
	GENERATED_BODY()

public:
	AACGameState();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

public:
	void AddTargetArray(class APlayerController* PlayerController);
	TArray<class APlayerController*> GetTargetArray() const;
	FORCEINLINE bool IsSplitScreen() const { return bIsSplitScreen; }
	void SetGamePhase(EACGamePhaseList NewPhase);
	void SetGameResult(EACGameResult NewResult);
	virtual void Tick(float DeltaTime) override;
	virtual void HandleMatchHasStarted() override;
	void SetupHUDWidget(class AACCharacterHUD* InHUDWidget);
	FORCEINLINE EACGamePhaseList GetCurrentGamePhase() const { return CurrentGamePhase; }
	FORCEINLINE EACGameResult GetGameResult() const { return CurrentGameResult; }
protected:
	// Victory

	// Defeat
	void OnPlayerTimerExpired();
	void OnAllPlayerDead();

public:
	FOnChangedGameResultDelegate OnChangedGameResult;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_ActiveGamePhaseObject)
	TObjectPtr<class AACGamePhase> ActiveGamePhaseObject;
	UPROPERTY(Replicated)
	TObjectPtr<class AACWaitingToStartPhase> WaitingToStartPhase;
	UPROPERTY(Replicated)
	TObjectPtr<class AACPlayerEntrancePhase> PlayerEntrancePhase;
	UPROPERTY(Replicated)
	TObjectPtr<class AACMinionCombatPhase> MinionCombatPhase;
	UPROPERTY(Replicated)
	TObjectPtr<class AACFinalCombatPhase> FinalCombatPhase;
	UPROPERTY(Replicated)
	TObjectPtr<class AACGameOverPhase> GameOverPhase;

	void OnChangeActiveGamePhase();
	void OnChangeGamePhase();
	void OnChangePrevGamePhase();
	void OnChangeGameResult();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentGamePhase)
	EACGamePhaseList CurrentGamePhase = EACGamePhaseList::WaitingToStart;
	UPROPERTY(ReplicatedUsing = OnRep_PrevGamePhase)
	EACGamePhaseList PrevGamePhase = EACGamePhaseList::GameOver;
	UPROPERTY(ReplicatedUsing = OnRep_GameResult)
	EACGameResult CurrentGameResult = EACGameResult::None;

	UFUNCTION()
	void OnRep_CurrentGamePhase();
	UFUNCTION()
	void OnRep_PrevGamePhase();
	UFUNCTION()
	void OnRep_GameResult();
	UFUNCTION()
	void OnRep_ActiveGamePhaseObject();

	uint8 bIsPlayersAllDead : 1;
	uint8 bIsTimerExpired : 1;

	uint8 bIsGamePhaseEned[ACGamePhaseUtility::ENUMNUM];

private:
	TSubclassOf<class AACGamePhase> WaitingToStartPhaseClass;
	TSubclassOf<class AACPlayerEntrancePhase> PlayerEntrancePhaseClass;
	TSubclassOf<class AACMinionCombatPhase> MinionCombatPhaseClass;
	TSubclassOf<class AACFinalCombatPhase> FinalCombatPhaseClass;
	TSubclassOf<class AACGameOverPhase> GameOverPhaseClass;

	TArray<class APlayerController*> TargetArray;
	uint8 bIsSplitScreen : 1;
};
