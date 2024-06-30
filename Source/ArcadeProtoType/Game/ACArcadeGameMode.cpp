#include "Game/ACArcadeGameMode.h"
#include "ACGameState.h"
#include "ACPlayerState.h"
#include "ACGameInstance.h"

#include "Character/ACCharacterBoss.h"
#include "Character/ACCharacterPlayer.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerStart.h"

#include "Game/ACGameState.h"
#include "Player/ACPlayerController.h"
#include "HUD/ACCharacterHUD.h"
#include "ArcadeProtoType.h"
#include "Interface/ACGamePhaseInterface.h"
#include "Interface/ACCharacterHUDInterface.h"

AACArcadeGameMode::AACArcadeGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> MantusClassRef(TEXT(
		"/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_CharacterMantus.BP_CharacterMantus_C'"));
	if (MantusClassRef.Class)
	{
		CharacterTypes.Add(MantusClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<APawn> CollieClassRef(TEXT(
		"/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_CharacterCollie.BP_CharacterCollie_C'"));
	if (CollieClassRef.Class)
	{
		CharacterTypes.Add(CollieClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArcadeProtoType.ACPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> DefaultCharacterHUDRef(TEXT(
		"/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_ACCharacterHUD.BP_ACCharacterHUD_C'"));
	if (DefaultCharacterHUDRef.Class)
	{
		HUDClass = DefaultCharacterHUDRef.Class;
	}


	DefaultPawnClass = MantusClassRef.Class;
	GameStateClass = AACGameState::StaticClass();
	PlayerStateClass = AACPlayerState::StaticClass();

	PlayerCount = 0;
	// bDelayedStart = true;
}

void AACArcadeGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AACArcadeGameMode::Logout(AController* Exiting) 
{
	Super::Logout(Exiting);
}

void AACArcadeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APawn* CurrentPawn = NewPlayer->GetPawn();
	if (CurrentPawn)
	{
		TArray<AActor*> AttachedActors;

		CurrentPawn->GetAttachedActors(AttachedActors);
		for (AActor* AttachedActor : AttachedActors) 
		{
			AttachedActor->Destroy();
		}
		CurrentPawn->Destroy();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;
	FVector SpawnLocation = FVector(88.f, 220.f, 290.f);

	const int NumberOfPlayers = GameState->PlayerArray.Num();
	const int CharacterTypeIndex = NumberOfPlayers - 1;

	SpawnLocation.Y += 100.f * CharacterTypeIndex;
	if (PlayerCount == 0) 
	{
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(CharacterTypes[PlayerCount], SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		NewPlayer->Possess(NewPawn);
		++PlayerCount;
	}
	else 
	{
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(CharacterTypes[PlayerCount], SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		NewPlayer->Possess(NewPawn);
		++PlayerCount;
	}

	if (GameState)
	{
		Cast<AACGameState>(GameState)->AddTargetArray(NewPlayer);
	}

	if (PlayerCount >= 2)
	{
		if (IACGamePhaseInterface* GamePhase = Cast< IACGamePhaseInterface>(GetWorld()->GetGameState()))
		{
			GamePhase->SetGamePhase(EACGamePhaseList::PlayerEntrance);
		}
	}
}

void AACArcadeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

void AACArcadeGameMode::BeginPlay()
{
	Super::BeginPlay();

}


void AACArcadeGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	if (MatchState == MatchState::InProgress) 
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (AACPlayerController* PC = Cast<AACPlayerController>(Iterator->Get()))
			{
				if (AACCharacterHUD* HUD = Cast<AACCharacterHUD>(PC->GetHUD()))
				{
					if (IACCharacterHUDInterface* HUDInterface = Cast<IACCharacterHUDInterface>(PC->PlayerState))
					{
						if (GameState->PlayerArray.Num() >= 2) 
						{
							HUDInterface->SetupHUDWidget(HUD);
						}
					}
				}
			}
		}
	}


	// UE_LOG(LogTemp, Log, TEXT("OnMatchStateSet"));

}

void AACArcadeGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);


	APawn* CurrentPawn = NewController->GetPawn();
	if (CurrentPawn)
	{
		TArray<AActor*> AttachedActors;

		CurrentPawn->GetAttachedActors(AttachedActors);
		for (AActor* AttachedActor : AttachedActors)
		{
			AttachedActor->Destroy();
		}
		CurrentPawn->Destroy();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewController;
	FVector SpawnLocation = FVector(88.f, 220.f, 290.f);

	const int NumberOfPlayers = GameState->PlayerArray.Num();
	const int CharacterTypeIndex = NumberOfPlayers - 1;

	SpawnLocation.Y += 100.f * CharacterTypeIndex;
	if (PlayerCount == 0)
	{
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(CharacterTypes[PlayerCount], SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		NewController->Possess(NewPawn);
		++PlayerCount;
	}
	else
	{
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(CharacterTypes[PlayerCount], SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		NewController->Possess(NewPawn);
		++PlayerCount;
	}

	if (GameState)
	{
		Cast<AACGameState>(GameState)->AddTargetArray(Cast<APlayerController>(NewController));
	}

	if (PlayerCount >= 2)
	{
		if (IACGamePhaseInterface* GamePhase = Cast< IACGamePhaseInterface>(GetWorld()->GetGameState()))
		{
			GamePhase->SetGamePhase(EACGamePhaseList::PlayerEntrance);
		}
	}
}

void AACArcadeGameMode::PlayerLeftGame(AACPlayerState* ArcadePlayerState)
{
	if (ArcadePlayerState == nullptr) 
	{
		return;
	}
	AACGameState* ArcadeGameState = GetGameState<AACGameState>();
	AACCharacterPlayer* CharacterLeaving = Cast<AACCharacterPlayer>(ArcadePlayerState->GetPawn());
	if (CharacterLeaving)
	{
		CharacterLeaving->Elim(true);
	}
}
