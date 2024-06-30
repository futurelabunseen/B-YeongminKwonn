#include "Game/GamePhase/ACFinalCombatPhase.h"
#include "Manager/ACSpawnManager.h"
#include "ArcadeProtoType.h"
#include "Game/ACGameState.h"

AACFinalCombatPhase::AACFinalCombatPhase()
{
}

void AACFinalCombatPhase::OnBeginPhase()
{
	const float MonsterStartDelay = 3.f;
	const float PlayerStartDelay  = 6.f;

	GetWorld()->GetTimerManager().SetTimer(MonsterDelayTimerHandle, this, &AACFinalCombatPhase::StartSpawnMosterAfterDelay, MonsterStartDelay, false);
	GetWorld()->GetTimerManager().SetTimer(PlayerDelayTimerHandle, this, &AACFinalCombatPhase::StartSpawnPlayerAfterDelay, PlayerStartDelay, false);
}

void AACFinalCombatPhase::OnPlayPhase(float DeltaTime)
{
	
}

void AACFinalCombatPhase::OnEndPhase()
{
	// PlayerSpawn
	if (UACSpawnManager* SpawnManager = GetWorld()->GetSubsystem<UACSpawnManager>())
	{
		SpawnManager->SetStopSpawn(true);
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (APawn* Player = Iterator->Get()->GetPawn())
		{
			Player->Destroy();
		}
	}
}

void AACFinalCombatPhase::StartSpawnMosterAfterDelay()
{
	// Boss
	if (UACSpawnManager* SpawnManager = GetWorld()->GetSubsystem<UACSpawnManager>())
	{
		SpawnManager->DoSpawn(EMonsterType::eBoss, EACGamePhaseList::FinalCombat);
	}

	// Minion
	if (UACSpawnManager* SpawnManager = GetWorld()->GetSubsystem<UACSpawnManager>())
	{
		const float SpawnRate = 60.f;
		SpawnManager->StartSpawn(EMonsterType::eMinion, EACGamePhaseList::FinalCombat);
	}

}

void AACFinalCombatPhase::StartSpawnPlayerAfterDelay()
{
	// PlayerSpawn
	if (UACSpawnManager* SpawnManager = GetWorld()->GetSubsystem<UACSpawnManager>()) 
	{
		if (AACGameState* ArcadeGameState = GetWorld()->GetAuthGameMode()->GetGameState<AACGameState>())
		{
			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				if (APawn* Player = Iterator->Get()->GetPawn())
				{
					SpawnManager->RequestRespawnCharacter(Player, ArcadeGameState->GetCurrentGamePhase());
				}
			}
		}
	}
}
