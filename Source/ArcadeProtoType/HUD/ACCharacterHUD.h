// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Data/ACDataUtility.h"
#include "Game/ACPlayerState.h"
#include "Game/GamePhase/ACGamePhase.h"
#include "Game/ACGameState.h"
#include "ACCharacterHUD.generated.h"


UENUM()
enum class EAimType : uint8
{
	DEFAULT UMETA(DisplayName = "Default"),
	AIM UMETA(DisplayName = "Aim"),
	DETECT UMETA(DisplayName = "Detect"),
};

enum class EACWidgetState : uint8
{
	WaitingToStart,	// Loading
	Default,			// Timer
	MinionCombat,		// Timer, HP, Ammo,		
	BossCombat,			// Timer, HP, Ammo,	BossHp		 
	Respawn,			// Timer, Respawn 
	Dead,				// Timer
	GameOver,			// GameOver
};

namespace ACWidgetStateUtility 
{
	static const int32 ENUMNUM = 6;

	static const int32 WaitingToStart = 0;
	static const int32 Default = 1;
	static const int32 MinionCombat = 2;
	static const int32 BossCombat = 3;
	static const int32 Respawn = 4;
	static const int32 Dead = 5;
	static const int32 GameOver = 6;

	static int32 WidgetStateToIndex(EACWidgetState WidgetState)
	{
		switch (WidgetState)
		{
		case EACWidgetState::WaitingToStart:
			return WaitingToStart;
		case EACWidgetState::Default:
			return Default;
		case EACWidgetState::MinionCombat:
			return MinionCombat;
		case EACWidgetState::BossCombat:
			return BossCombat;
		case EACWidgetState::Respawn:
			return Respawn;
		case EACWidgetState::Dead:
			return Dead;
		case EACWidgetState::GameOver:
			return GameOver;
		default:
			return -1;
		}
	}
};

UCLASS()
class ARCADEPROTOTYPE_API AACCharacterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AACCharacterHUD();

	// DelegateEventFunction
public:
	void UpdateHpWidget(float NewCurrentHP, float NewmaxHp);
	void UpdateAmmoWidget(int NewAmmoCount, int MaxAmmoCount);
	void UpdateMonsterHpBar(float NewCurrentHP, float NewmaxHp);
	void UpdateRespawnWidget(float percent, float SpawnTime);
	void UpdateTimeTextWidget(FString TimeText);
	void UpdateAimStatus(EAimType NewAimType);
	void UpdateHUDPlayerState(EACPlayerState IN NewPlayerState);
	void UpdateHUDGamePhaseState(EACGamePhaseList IN NewPlayerState);
	void RemoveBossHPBarWidget();
	void UpdateGameOverWidget(EACGameResult GameResultString);


protected:
	void RemoveFromParent(class UACUserWidget* Widget);
	virtual void BeginPlay() override;
	void UpdateAimAssistWidget();
	void ToggleUI(int32 Index);

protected:
	UPROPERTY()
	TSubclassOf<class UACHpWidget> HpUIWidgetClass;

	UPROPERTY()
	TSubclassOf<class UACAmmoWidget> AmmoUIWidgetClass;

	UPROPERTY()
	TSubclassOf<class UACAimAssistWidget> AimAssistUIWidgetClass;

	UPROPERTY()
	TSubclassOf<class UACTargetDetectWidget> DetectUIWidgetClass;

	UPROPERTY()
	TSubclassOf<class UACHpBarWidget> MonsterHpWidgetClass;

	UPROPERTY()
	TSubclassOf<class UACRespawnWidget> RespawnUIWidgetClass;

	UPROPERTY()
	TSubclassOf<class UACTimerWidget> TimerUIWidgetClass;

	UPROPERTY()
	TSubclassOf<class UACLoadingWidget> LoadingUIWidgetClass;

	UPROPERTY()
	TSubclassOf<class UACGameOverWidget> GameOverUIWidgetClass;

	UPROPERTY()
	TObjectPtr<UACHpWidget> HpUIWidget;

	UPROPERTY()
	TObjectPtr<UACAmmoWidget> AmmoUIWidget;

	UPROPERTY()
	TObjectPtr<UACAimAssistWidget> AimAssistUIWidget;

	UPROPERTY()
	TObjectPtr<UACTargetDetectWidget> DetectUIWidget;

	UPROPERTY()
	TObjectPtr<UACHpBarWidget> MonsterHpWidget;

	UPROPERTY()
	TObjectPtr<class UACTimerWidget> TimerUIWidget;

	UPROPERTY()
	TObjectPtr<class UACRespawnWidget> RespawnUIWidget;

	UPROPERTY()
	TObjectPtr<class UACLoadingWidget> LoadingUIWidget;

	UPROPERTY()
	TObjectPtr<class UACGameOverWidget> GameOverUIWidget;

private:
	TArray<TObjectPtr<class UACUserWidget>> GameUIContainers[ACWidgetStateUtility::ENUMNUM];
	
	EACWidgetState CurrentWidgetState = EACWidgetState::WaitingToStart;
	EACPlayerState CurrentPlayerState = EACPlayerState::Alive;
	EACGamePhaseList CurrentGamePhase;

	void SetHUDState();
	void ToggleCombatWidgets(ESlateVisibility inVisibility);
	uint8 bIsDetecting : 1;
	uint8 bIsAiming : 1;
	EAimType CurrentAimType;

};
