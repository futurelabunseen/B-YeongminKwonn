#include "UI/ACGameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MultiplayerSessions.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Character/ACCharacterPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UACGameOverWidget::UACGameOverWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UACGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UACGameOverWidget::ClickMainMenuButton);
	}
}

void UACGameOverWidget::UpdateResultText(FString Result)
{
	GameResultText->SetText(FText::FromString(Result));
}

void UACGameOverWidget::SetupWidget()
{
	bIsFocusable = true;

	if (UWorld* World = GetWorld())
	{
		PlayerController = World->GetFirstPlayerController();

		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}

		return;

		UGameInstance* GameInstance = GetGameInstance();
		if (GameInstance)
		{
			MultiplayerSessionsSubsystem = GameInstance->GetSubsystem <class UMultiplayerSessionsSubsystem>();
			if (MultiplayerSessionsSubsystem)
			{
				MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UACGameOverWidget::OnDestroySession);
			}
		}
	}

	// GetWorld()->GetWorldSettings()->SetTimeDilation(0.f);
	// GetWorld()->GetAuthGameMode()->HandleLeavingMap();
}

void UACGameOverWidget::ClickMainMenuButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("GameTitle"));
	return;
	 MainMenuButton->SetIsEnabled(false);
	 
	 UWorld* World = GetWorld();
	 if (World)
	 {
	 	APlayerController* FirstPlayerController = World->GetFirstPlayerController();
	 	if (FirstPlayerController)
	 	{
	 		if(AACCharacterPlayer* Player = Cast<AACCharacterPlayer>(FirstPlayerController->GetPawn()))
	 		{
	 			Player->ServerRPCLeaveGame();
	 			Player->OnLeftGame.AddUObject(this, &UACGameOverWidget::OnPlayerLeftGame);
	 		}
	 		else
	 		{
	 			MainMenuButton->SetIsEnabled(true);
	 		}
	 	}
	 }
}

void UACGameOverWidget::OnDestroySession(bool bWasSuccessful)
{

	if (!bWasSuccessful)
	{
		MainMenuButton->SetIsEnabled(true);
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			PlayerController = World->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UACGameOverWidget::OnPlayerLeftGame()
{
	if (MultiplayerSessionsSubsystem)
	{
			MultiplayerSessionsSubsystem->DestroySession();
	}
}

void UACGameOverWidget::ClickExitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

