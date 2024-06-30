
#include "HUD/ACCharacterHUD.h"
#include "Interface/ACCharacterHUDInterface.h"
#include "Character/ACCharacterPlayer.h"
#include "Character/ACCharacterBoss.h"
#include "Kismet/GameplayStatics.h"
#include "ArcadeProtoType.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Components/ACShootingComponent.h"

#include "UI/ACHpWidget.h"
#include "UI/ACTargetDetectWidget.h"
#include "UI/ACAimAssistWidget.h"
#include "UI/ACHpBarWidget.h"
#include "UI/ACAmmoWidget.h"
#include "UI/ACRespawnWidget.h"
#include "UI/ACTimerWidget.h"
#include "UI/ACLoadingWidget.h"
#include "UI/ACGameOverWidget.h"
#include "Engine.h"

#include "Player/ACPlayerController.h"
#include "Manager/ACMonsterManager.h"
#include "Manager/ACDataManager.h"

AACCharacterHUD::AACCharacterHUD()
{
	static ConstructorHelpers::FClassFinder<UACHpWidget> HpUIWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Arcade/UI/WBP_HP.WBP_HP_C'"));
	if (HpUIWidgetClassRef.Class)
	{
		HpUIWidgetClass = HpUIWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UACAimAssistWidget> AimAssistUIWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Arcade/UI/WBP_AimAssist.WBP_AimAssist_C'"));
	if (AimAssistUIWidgetClassRef.Class)
	{
		AimAssistUIWidgetClass = AimAssistUIWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UACTargetDetectWidget> DetectUIWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Arcade/UI/WBP_TargetDetect.WBP_TargetDetect_C'"));
	if (DetectUIWidgetClassRef.Class)
	{
		DetectUIWidgetClass = DetectUIWidgetClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UACHpBarWidget> MonsterHpWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Arcade/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (MonsterHpWidgetClassRef.Class)
	{
		MonsterHpWidgetClass = MonsterHpWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UACAmmoWidget> AmmoWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Arcade/UI/WBP_Ammo.WBP_Ammo_C'"));
	if (AmmoWidgetClassRef.Class)
	{
		AmmoUIWidgetClass = AmmoWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UACRespawnWidget> RespawnWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Arcade/UI/WBP_RespawnWidget.WBP_RespawnWidget_C'"));
	if (RespawnWidgetClassRef.Class)
	{
		RespawnUIWidgetClass = RespawnWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UACTimerWidget> TimerWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Arcade/UI/WBP_Timer.WBP_Timer_C'"));
	if (TimerWidgetClassRef.Class)
	{
		TimerUIWidgetClass = TimerWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UACLoadingWidget> LoadingWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Arcade/UI/WBP_LoadingWidget.WBP_LoadingWidget_C'"));
	if (LoadingWidgetClassRef.Class)
	{
		LoadingUIWidgetClass = LoadingWidgetClassRef.Class;
		UE_LOG(LogTemp, Log, TEXT("LoadingUIWidgetClass Loading Success"));
	}
	else 
	{
		UE_LOG(LogTemp, Log, TEXT("LoadingUIWidgetClass Loading Failed"));
	}

	static ConstructorHelpers::FClassFinder<UACGameOverWidget> GameOverWidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Arcade/UI/WBP_GameOverWidget.WBP_GameOverWidget_C'"));
	if (GameOverWidgetClassRef.Class)
	{
		GameOverUIWidgetClass = GameOverWidgetClassRef.Class;
		
	}
	
	CurrentAimType = EAimType::DEFAULT;
}

void AACCharacterHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerOwner = GetOwningPlayerController();

	if (PlayerOwner == nullptr) 
	{
		return;
	}

	if (IACCharacterHUDInterface* PC = Cast<IACCharacterHUDInterface>(PlayerOwner)) 
	{
		PC->SetupHUDWidget(this);
	}

	if(IACCharacterHUDInterface* Player = Cast<IACCharacterHUDInterface>(PlayerOwner->GetPawn()))
	{
		Player->SetupHUDWidget(this);
	}

	if(IACCharacterHUDInterface* GS = Cast<IACCharacterHUDInterface>(GetWorld()->GetGameState()))
	{
		GS->SetupHUDWidget(this);
	}


	if (HpUIWidgetClass)
	{
		AACCharacterPlayer* Player = Cast< AACCharacterPlayer>(PlayerOwner->GetPawn());
		if (Player == nullptr) 
		{
			return;
		}
		HpUIWidget = CreateWidget<UACHpWidget>(PlayerOwner, HpUIWidgetClass, TEXT("HpWidget"));
		if (HpUIWidget)
		{
			HpUIWidget->AddToViewport();
			HpUIWidget->SetVisibility(ESlateVisibility::Hidden);
			if (Player) 
			{
				if (UACDataManager* DataManager = UGameInstance::GetSubsystem<UACDataManager>(GetWorld()->GetGameInstance()))
				{
					if (true == DataManager->IsCharacterImageValid()) 
					{
						FACCharacterImage MantusImage = DataManager->GetCharacterImageData(0);
						FACCharacterImage CollieImage = DataManager->GetCharacterImageData(1);

						switch (Player->GetCharacterType())
						{
						case ECharacterType::eMantus:
							//HpUIWidget->SetImagePath(L"/Script/Engine.Texture2D'/Game/Arcade/UI/Textures/MantusProfileImage.MantusProfileImage'");
							HpUIWidget->SetImageTexture(MantusImage.CharacterProfileTexture);
							break;
						case ECharacterType::eCollie:
							//HpUIWidget->SetImagePath(L"/Script/Engine.Texture2D'/Game/Arcade/UI/Textures/CollieProfileImage.CollieProfileImage'");
							HpUIWidget->SetImageTexture(CollieImage.CharacterProfileTexture);
							break;
						default:
							break;
						}
					}
				}

				
			}
			UpdateHpWidget(100.f, 100.f);
		}
	}

	if (AmmoUIWidgetClass) 
	{
		AmmoUIWidget = CreateWidget<UACAmmoWidget>(PlayerOwner, AmmoUIWidgetClass, TEXT("AmmoWidget"));
		if (AmmoUIWidget) 
		{
			AmmoUIWidget->AddToViewport();
			AmmoUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (TimerUIWidgetClass)
	{
		TimerUIWidget = CreateWidget<UACTimerWidget>(PlayerOwner, TimerUIWidgetClass, TEXT("TimerWidget"));
		if (TimerUIWidget)
		{
			TimerUIWidget->AddToViewport();
		}
		
	}

	if (AimAssistUIWidgetClass)
	{
		AimAssistUIWidget = CreateWidget<UACAimAssistWidget>(PlayerOwner, AimAssistUIWidgetClass, TEXT("AimAssistWidget"));
		if (AimAssistUIWidget)
		{
			AimAssistUIWidget->AddToViewport();
		}
	}

	if (DetectUIWidgetClass)
	{
		DetectUIWidget = CreateWidget<UACTargetDetectWidget>(PlayerOwner, DetectUIWidgetClass, TEXT("DetectWidget"));
		if (DetectUIWidget)
		{
			DetectUIWidget->AddToViewport(); 
			DetectUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (MonsterHpWidgetClass)
	{
		MonsterHpWidget = CreateWidget<UACHpBarWidget>(PlayerOwner, MonsterHpWidgetClass, TEXT("MonsterHpWidget"));
		if (MonsterHpWidget)
		{
			MonsterHpWidget->AddToViewport();
			MonsterHpWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (RespawnUIWidgetClass)
	{
		RespawnUIWidget = CreateWidget<UACRespawnWidget>(PlayerOwner, RespawnUIWidgetClass, TEXT("RespawnWidget"));
		if (RespawnUIWidget)
		{
			RespawnUIWidget->AddToViewport();
			RespawnUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (LoadingUIWidgetClass)
	{
		LoadingUIWidget = CreateWidget<UACLoadingWidget>(PlayerOwner, LoadingUIWidgetClass, TEXT("LoadingWidget"));
		if (LoadingUIWidget)
		{
			LoadingUIWidget->AddToViewport();
		}
	}

	if (GameOverUIWidgetClass)
	{
		GameOverUIWidget = CreateWidget<UACGameOverWidget>(PlayerOwner, GameOverUIWidgetClass, TEXT("GameOverWidget"));
		if (GameOverUIWidget)
		{
			GameOverUIWidget->AddToViewport();
			GameOverUIWidget->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Log, TEXT("GameOverUIWidget Create"));
		}
		else 
		{
			UE_LOG(LogTemp, Log, TEXT("GameOverUIWidget Not Create"));
		}
	}

	UpdateAimStatus(CurrentAimType);
}

/*
	추후에 회복 상태면, tint 컬러가 바뀌도록 구현하고 싶다
*/
void AACCharacterHUD::UpdateHpWidget(float NewCurrentHP, float NewmaxHp)
{
	const float NewPercent = FMath::Clamp((NewCurrentHP / NewmaxHp), 0.0f, 1.0f);
	if (HpUIWidget) 
	{
		HpUIWidget->UpdateHPWidget(NewPercent);
	}
}

void AACCharacterHUD::UpdateAmmoWidget(int NewAmmoCount, int MaxAmmoCount)
{
	const float NewPercent = FMath::Clamp(((float)NewAmmoCount / (float)MaxAmmoCount), 0.0f, 1.0f);

	if (AmmoUIWidget)
	{
		AmmoUIWidget->UpdateHPWidget(NewPercent);
	}
}

void AACCharacterHUD::UpdateAimStatus(EAimType NewAimType)
{
	CurrentAimType = NewAimType;

	switch (CurrentAimType)
	{
	case EAimType::DEFAULT:
		AimAssistUIWidget->UpdateAimAssistWidget(false);
		DetectUIWidget->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EAimType::AIM:
		AimAssistUIWidget->UpdateAimAssistWidget(true);
		DetectUIWidget->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EAimType::DETECT:
		AimAssistUIWidget->UpdateAimAssistWidget(true);
		DetectUIWidget->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}
}

void AACCharacterHUD::UpdateHUDPlayerState(EACPlayerState IN NewPlayerState)
{
	CurrentPlayerState = NewPlayerState;
	SetHUDState();
}

void AACCharacterHUD::UpdateHUDGamePhaseState(EACGamePhaseList IN NewGamePhase)
{
	CurrentGamePhase = NewGamePhase;
	SetHUDState();
}

void AACCharacterHUD::RemoveBossHPBarWidget()
{
	RemoveFromParent(MonsterHpWidget);
}

void AACCharacterHUD::RemoveFromParent(UACUserWidget* Widget)
{
	if (Widget) 
	{
		Widget->RemoveFromParent();
	}
}

void AACCharacterHUD::SetHUDState()
{
	if (CurrentGamePhase == EACGamePhaseList::WaitingToStart) 
	{
		if (LoadingUIWidget) 
		{
			LoadingUIWidget->SetVisibility(ESlateVisibility::Visible);
		}
		if (MonsterHpWidget)
		{
			MonsterHpWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TimerUIWidget)
		{
			TimerUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (RespawnUIWidget)
		{
			RespawnUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (GameOverUIWidget)
		{
			GameOverUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		ToggleCombatWidgets(ESlateVisibility::Hidden);
	}
	else if (CurrentGamePhase == EACGamePhaseList::PlayerEntrance) 
	{
		if (LoadingUIWidget) 
		{
			LoadingUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (MonsterHpWidget)
		{
			MonsterHpWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TimerUIWidget) 
		{
			TimerUIWidget->SetVisibility(ESlateVisibility::Visible);
		}
		if (RespawnUIWidget)
		{
			RespawnUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (GameOverUIWidget)
		{
			GameOverUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		ToggleCombatWidgets(ESlateVisibility::Hidden);
	}
	else if (CurrentGamePhase == EACGamePhaseList::MinionCombat || CurrentGamePhase == EACGamePhaseList::FinalCombat)
	{
		if (LoadingUIWidget)
		{
			LoadingUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (GameOverUIWidget)
		{
			GameOverUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TimerUIWidget)
		{
			TimerUIWidget->SetVisibility(ESlateVisibility::Visible);
		}

		if (CurrentGamePhase == EACGamePhaseList::MinionCombat) 
		{
			if (MonsterHpWidget) 
			{
				MonsterHpWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else 
		{
			if (MonsterHpWidget) 
			{
				MonsterHpWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}

		if (CurrentPlayerState == EACPlayerState::Alive) 
		{
			if (RespawnUIWidget) 
			{
				RespawnUIWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			ToggleCombatWidgets(ESlateVisibility::Visible);
		}
		else if (CurrentPlayerState == EACPlayerState::Dead)
		{
			if (RespawnUIWidget)
			{
				RespawnUIWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			ToggleCombatWidgets(ESlateVisibility::Hidden);
		}
		else 
		{
			if (RespawnUIWidget) 
			{
				RespawnUIWidget->SetVisibility(ESlateVisibility::Visible);
			}
			ToggleCombatWidgets(ESlateVisibility::Hidden);
		}
	}
	else if (CurrentGamePhase == EACGamePhaseList::GameOver) 
	{
		if (LoadingUIWidget) 
		{
			LoadingUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (MonsterHpWidget) 
		{
			MonsterHpWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TimerUIWidget)
		{
			TimerUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (RespawnUIWidget)
		{
			RespawnUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (GameOverUIWidget) 
		{
			GameOverUIWidget->SetVisibility(ESlateVisibility::Visible);
		}
		ToggleCombatWidgets(ESlateVisibility::Hidden);
	}
}

void AACCharacterHUD::UpdateGameOverWidget(EACGameResult GameResultString)
{
	FString GameResult = GameResultString == EACGameResult::Victory ? TEXT("Victory") : TEXT("Defeat");
	GameOverUIWidget->SetupWidget();
		GameOverUIWidget->UpdateResultText(GameResult); 
}

void AACCharacterHUD::ToggleCombatWidgets(ESlateVisibility inVisibility)
{
	if (HpUIWidget)
	{
		HpUIWidget->SetVisibility(inVisibility);
	}
	if (AmmoUIWidget)
	{
		AmmoUIWidget->SetVisibility(inVisibility);
	}
	if (AimAssistUIWidget)
	{
		AimAssistUIWidget->SetVisibility(inVisibility);
	}
}

void AACCharacterHUD::UpdateAimAssistWidget()
{
	if (CurrentAimType != EAimType::DEFAULT) 
	{
		AimAssistUIWidget->UpdateAimAssistWidget(true);
	}
	else 
	{
		AimAssistUIWidget->UpdateAimAssistWidget(false);
	}
}

void AACCharacterHUD::ToggleUI(int32 Index)
{
	for (int32 i = 0; i < ACWidgetStateUtility::ENUMNUM; ++i)
	{
		if (Index == i) 
		{
			for (UACUserWidget * Widget : GameUIContainers[i])
			{
				Widget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else 
		{
			for (UACUserWidget* Widget : GameUIContainers[i])
			{
				Widget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void AACCharacterHUD::UpdateMonsterHpBar(float NewCurrentHP, float NewmaxHp)
{
	const float NewPercent = FMath::Clamp((NewCurrentHP / NewmaxHp), 0.0f, 1.0f);
	UE_LOG(LogTemp, Log, TEXT("NewPercent : %f"), NewPercent);
	if (MonsterHpWidget)
	{
		MonsterHpWidget->UpdateHpBar(NewPercent);
	}
}

void AACCharacterHUD::UpdateRespawnWidget(float percent, float SpawnTime)
{
	const float ScaledPercent = (percent / SpawnTime);

	if (RespawnUIWidget)
	{
		RespawnUIWidget->UpdateWidget(ScaledPercent);
	}
}

void AACCharacterHUD::UpdateTimeTextWidget(FString TimeText)
{
	if (TimerUIWidget) 
	{
		TimerUIWidget->UpdateTimerTextWidget(TimeText);
	}
}
