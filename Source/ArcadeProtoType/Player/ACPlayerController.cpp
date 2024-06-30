
#include "Player/ACPlayerController.h"
#include "UI/ACHUDWidget.h"
#include "Game/ACArcadeGameMode.h"
#include "Interface/ACAISpawner.h"
#include "HUD/ACCharacterHUD.h"
#include "Manager/ACDataManager.h"
#include "kismet/GameplayStatics.h"

AACPlayerController::AACPlayerController()
{
	bIsClinetReady = false;
	TimeText = TEXT("");
}

void AACPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	++TimeSyncRunningTime;
	SetSyncBetweenServerAndClient(DeltaTime);
}


void AACPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		if (true == DataManager->IsTimerDataValid())
		{
			FACTimerData TimerData = DataManager->GetTimerData(0);
			TimerCheckRate = TimerData.TimerCheckRate;
			TotalTime = TimerData.TotalTime;
		}
	}

	if (AACCharacterHUD* HUD = Cast<AACCharacterHUD>(GetHUD()))
	{
		SetupHUDWidget(HUD);
	}

	// if (GetWorld()) 
	// {
	// 	if (GetWorld()->GetAuthGameMode())
	// 	{
	// 		if (AACArcadeGameMode* GameMode = Cast<AACArcadeGameMode>(GetWorld()->GetAuthGameMode()))
	// 		{
	// 			GameMode->CountPlayers();
	// 		}
	// 	}
	// }


	if (IsLocalController())
	{
		// 1초마다 SetTimeInHUD를 호출하도록 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &AACPlayerController::UpdateTime, 1.0f, true);
	}



	// if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	//{
		// FString OutError;
		// GEngine->GameViewportClientClass = UGameViewportClient::StaticClass();

		// UCommonGameViewportClient

		// UGameViewportClient* ViewportClient = GEngine->GameViewport;
		//n ViewportClient->SplitscreenInfo[ESplitScreenType::None].PlayerData[0] = FPerPlayerSplitscreenData(0.5f, 1.0f, 0.0f, 0.0f);
	//}
}

void AACPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AACPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AACPlayerController::SetSyncBetweenServerAndClient(float DeltaTime)
{
	if (IsLocalController() && TimeSyncRunningTime > TimerCheckRate)
	{
		ServerRPCRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0;
	}
}

void AACPlayerController::ServerRPCRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfRecipt = GetWorld()->GetTimeSeconds();
	ClientRPCReportServerTime(TimeOfClientRequest , ServerTimeOfRecipt);
}

void AACPlayerController::ClientRPCReportServerTime_Implementation(float TimeOfClientRequest,float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}


void AACPlayerController::SetupHUDWidget(AACCharacterHUD* InHUDWidget)
{
	if (InHUDWidget) 
	{
		InHUDWidget->UpdateTimeTextWidget(TimeText);
		OnTimeChanged.AddUObject(InHUDWidget, &AACCharacterHUD::UpdateTimeTextWidget);
	}
}

void AACPlayerController::UpdateTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(TotalTime - GetServerTime());
	if (CountdownInt != SecondsLeft)
	{
		UpdateTimeText(TotalTime - GetServerTime());
	}
	CountdownInt = SecondsLeft;

	if(CountdownInt <= 0)
	{
		if (OnTimerExpired.IsBound()) 
		{
			OnTimerExpired.Broadcast();
		}
		GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
	}
}

void AACPlayerController::UpdateTimeText(float Time)
{
	int32 Minutes = FMath::FloorToInt(Time / 60.f);
	int32 Seconds = Time - Minutes * 60;
	TimeText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	
	if (OnTimeChanged.IsBound()) 
	{
		OnTimeChanged.Broadcast(TimeText);
	}
}

float AACPlayerController::GetServerTime()
{
	if (true == HasAuthority()) 
	{
		return GetWorld()->GetTimeSeconds(); 
	}
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AACPlayerController::CreateServer()
{
	
}

void AACPlayerController::JoinServer()
{

}

void AACPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		ServerRPCRequestServerTime(GetWorld()->GetTimeSeconds());
		CountdownInt = FMath::CeilToInt(TotalTime - GetServerTime());
	}
}