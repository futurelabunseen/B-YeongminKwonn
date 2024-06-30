#include "ACMinionCombatPhase.h"
#include "Manager/ACSpawnManager.h"
#include "Manager/ACMonsterManager.h"
#include "Interface/ACGameModePhaseInterface.h"
#include "Game/ACGameState.h"


AACMinionCombatPhase::AACMinionCombatPhase()
{
	bStopCombat = false;
}

void AACMinionCombatPhase::OnBeginPhase()
{
	UE_LOG(LogTemp, Log, TEXT("UACMinionCombatPhase::OnBeginPhase"));

	// Minion
	// Minion을 스폰한다.
	if (UACSpawnManager* SpawnManager = GetWorld()->GetSubsystem<UACSpawnManager>())
	{
		SpawnManager->StartSpawn(EMonsterType::eMinion, EACGamePhaseList::MinionCombat);
	}

	// Minon이 죽으면 전투를 끝낸다.
	if (UACMonsterManager* MonsterManager = GetWorld()->GetSubsystem<UACMonsterManager>())
	{
			MonsterManager->OnAllMonsterDead.AddUObject(this, &AACMinionCombatPhase::ChangeGamePhase);
	}
	// HP, Ammo, AimAssist 이미지를 Visibility 한다.
}

void AACMinionCombatPhase::OnPlayPhase(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("UACMinionCombatPhase::OnPlayPhase"));
}

void AACMinionCombatPhase::OnEndPhase()
{
	// HP, Ammo, AimAssist 이미지를 Hidden 한다.
}

void AACMinionCombatPhase::ChangeGamePhase()
{
	if (AACGameState* GS = Cast<AACGameState>(GetWorld()->GetAuthGameMode()->GameState))
	{
		GS->SetGamePhase(EACGamePhaseList::FinalCombat);
		bStopCombat = false;
	}
}