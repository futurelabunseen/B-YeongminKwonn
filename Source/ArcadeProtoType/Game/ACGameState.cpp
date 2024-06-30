#include "Game/ACGameState.h"
#include "Game/ACPlayerState.h"
#include "Player/ACPlayerController.h"
#include "ArcadeProtoType.h"
#include "Net/UnrealNetwork.h"

#include "GamePhase/ACFinalCombatPhase.h"
#include "GamePhase/ACGameOverPhase.h"
#include "GamePhase/ACMinionCombatPhase.h"
#include "GamePhase/ACPlayerEntrancePhase.h"
#include "GamePhase/ACWaitingToStartPhase.h"

#include "HUD/ACCharacterHUD.h"


AACGameState::AACGameState()
{
	bIsSplitScreen = false;

	// static ConstructorHelpers::FClassFinder<AACWaitingToStartPhase> WaitingToStartPhaseClassRef(TEXT("/Script/Engine.Blueprint'/Game/Arcade/Blueprint/GamePhase/BP_WaitingToStart.BP_WaitingToStart_C'"));
	// if (WaitingToStartPhaseClassRef.Class)
	// {
	// 	WaitingToStartPhaseClass = WaitingToStartPhaseClassRef.Class;
	// }

	// static ConstructorHelpers::FClassFinder<AACPlayerEntrancePhase> PlayerEntrancePhaseClassRef(TEXT("/Script/Engine.Blueprint'/Game/Arcade/Blueprint/GamePhase/BP_PlayerEntrance.BP_PlayerEntrance_C'"));
	// if (PlayerEntrancePhaseClassRef.Class)
	// {
	// 	PlayerEntrancePhaseClass = PlayerEntrancePhaseClassRef.Class;
	// }
	
	WaitingToStartPhase = CreateDefaultSubobject<AACWaitingToStartPhase>(TEXT("WaitingToStartPhase"));
	PlayerEntrancePhase = CreateDefaultSubobject<AACPlayerEntrancePhase>(TEXT("PlayerEntrancePhase"));
	MinionCombatPhase = CreateDefaultSubobject<AACMinionCombatPhase>(TEXT("MinionCombatPhase"));
	FinalCombatPhase = CreateDefaultSubobject<AACFinalCombatPhase>(TEXT("FinalCombatPhase"));
	GameOverPhase = CreateDefaultSubobject<AACGameOverPhase>(TEXT("GameOverPhase"));

}

void AACGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACGameState, CurrentGamePhase);
	DOREPLIFETIME(AACGameState, PrevGamePhase);
	DOREPLIFETIME(AACGameState, CurrentGameResult);

	// DOREPLIFETIME(AACGameState, ActiveGamePhaseObject);
	// DOREPLIFETIME(AACGameState, WaitingToStartPhase);
	// DOREPLIFETIME(AACGameState, PlayerEntrancePhase);
	// DOREPLIFETIME(AACGameState, MinionCombatPhase);
	// DOREPLIFETIME(AACGameState, FinalCombatPhase);
	// DOREPLIFETIME(AACGameState, GameOverPhase);
	
	DOREPLIFETIME_CONDITION(AACGameState, ActiveGamePhaseObject, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AACGameState, WaitingToStartPhase, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AACGameState, PlayerEntrancePhase, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AACGameState, MinionCombatPhase, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AACGameState, FinalCombatPhase, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AACGameState, GameOverPhase, COND_SimulatedOnly); 
}

void AACGameState::SetGamePhase(EACGamePhaseList NewPhase)
{
	if (HasAuthority()) 
	{
		PrevGamePhase = CurrentGamePhase;
		OnChangePrevGamePhase();

		CurrentGamePhase = NewPhase;
		OnChangeGamePhase();
	}
}

void AACGameState::SetGameResult(EACGameResult NewResult)
{
	if (HasAuthority()) 
	{
		CurrentGameResult = NewResult;
		OnChangeGameResult();
	}
}

void AACGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveGamePhaseObject)
	{
		ActiveGamePhaseObject->OnPlayPhase(DeltaTime);
	}
}

void AACGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) 
	{
		if (AACPlayerController* PC = Cast<AACPlayerController>(Iterator->Get()))
		{
			PC->OnTimerExpired.AddUObject(this, &AACGameState::OnPlayerTimerExpired);
		}
	}
}

void AACGameState::SetupHUDWidget(AACCharacterHUD* InHUDWidget)
{
	if (InHUDWidget) 
	{
		OnChangedGamePhase.AddUObject(InHUDWidget, &AACCharacterHUD::UpdateHUDGamePhaseState);
		OnChangedGameResult.AddUObject(InHUDWidget, &AACCharacterHUD::UpdateGameOverWidget);
	}
}


void AACGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FActorSpawnParameters SpawnParams;
	// WaitingToStartPhase = GetWorld()->SpawnActor<AACWaitingToStartPhase>(WaitingToStartPhaseClass, SpawnParams); // NewObject<AACWaitingToStartPhase>(this, AACWaitingToStartPhase::StaticClass());
	// PlayerEntrancePhase = GetWorld()->SpawnActor<AACPlayerEntrancePhase>(PlayerEntrancePhaseClass, SpawnParams); // NewObject<AACWaitingToStartPhase>(this, AACWaitingToStartPhase::StaticClass());

	SetGamePhase(CurrentGamePhase);

	for (APlayerState* PlayerStateFromArray : PlayerArray)
	{
		if (AACPlayerState* PS = Cast<AACPlayerState>(PlayerStateFromArray))
		{
			PS->OnChangedPlayerState.AddUObject(this, &AACGameState::OnAllPlayerDead);
		}
	}
}

void AACGameState::OnRep_ActiveGamePhaseObject()
{
	OnChangeActiveGamePhase();
}

void AACGameState::OnChangeActiveGamePhase()
{
	if (ActiveGamePhaseObject)
	{
		ActiveGamePhaseObject->OnBeginPhase();
	}
}

void AACGameState::OnChangeGamePhase()
{
	switch (CurrentGamePhase)
	{
	case EACGamePhaseList::WaitingToStart:
		ActiveGamePhaseObject = WaitingToStartPhase;
		break;
	case EACGamePhaseList::PlayerEntrance:
		ActiveGamePhaseObject = Cast<AACPlayerEntrancePhase>(PlayerEntrancePhase);
		break;
	case EACGamePhaseList::MinionCombat:
		ActiveGamePhaseObject = Cast<AACMinionCombatPhase>(MinionCombatPhase);
		break;
	case EACGamePhaseList::FinalCombat:
		ActiveGamePhaseObject = Cast<AACFinalCombatPhase>(FinalCombatPhase);
		break;
	case EACGamePhaseList::GameOver:
		ActiveGamePhaseObject = Cast<AACGameOverPhase>(GameOverPhase);
		break;
	default:break;
	}

	OnChangeActiveGamePhase();

	if (OnChangedGamePhase.IsBound()) 
	{
		OnChangedGamePhase.Broadcast(CurrentGamePhase);
	}
}

void AACGameState::OnChangePrevGamePhase()
{
	if (HasAuthority()) 
	{
		if (ActiveGamePhaseObject)
		{
			ActiveGamePhaseObject->OnEndPhase();
		}
	}
}

void AACGameState::OnRep_CurrentGamePhase()
{
	OnChangeGamePhase();
}

void AACGameState::OnRep_PrevGamePhase()
{
	OnChangePrevGamePhase();
}


void AACGameState::OnChangeGameResult() 
{
	if (OnChangedGamePhase.IsBound()) 
	{
		OnChangedGamePhase.Broadcast(CurrentGamePhase);
	}
	if (OnChangedGameResult.IsBound())
	{
		OnChangedGameResult.Broadcast(CurrentGameResult);
	}
}

void AACGameState::OnRep_GameResult()
{
	OnChangeGameResult();
}

// Defeat Condition //
void AACGameState::OnPlayerTimerExpired()
{
	SetGamePhase(EACGamePhaseList::GameOver);
	SetGameResult(EACGameResult::Defeat);
}

void AACGameState::OnAllPlayerDead()
{
	int32 AliveCount = 0;

	for (APlayerState* PlayerState : PlayerArray)
	{
		if (AACPlayerState* PS = Cast<AACPlayerState>(PlayerState))
		{
			if (PS && (static_cast<uint8>(PS->GetCurrentPlayerState()) & static_cast<uint8>(EACPlayerState::Alive)))
			{
				AliveCount++;
			}
		}
	}

	if (AliveCount == 0) 
	{
		SetGameResult(EACGameResult::Defeat); 
	}
}

//////////////////////////////// Target ////////////////////////////////
void AACGameState::AddTargetArray(APlayerController* PlayerController)
{
	TargetArray.Add(PlayerController);
}

TArray<class APlayerController*> AACGameState::GetTargetArray() const
{
	return TargetArray;
}