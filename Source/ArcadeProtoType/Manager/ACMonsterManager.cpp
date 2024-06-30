#include "Manager/ACMonsterManager.h"
#include "Manager/ACDatamanager.h"
#include "Character/ACCharacterMinion.h"
#include "Character/ACCharacterBoss.h"
#include "HUD/ACCharacterHUD.h"
#include "ArcadeProtoType.h"
#include "Game/ACGameState.h"

UACMonsterManager::UACMonsterManager() 
{
	bIsAllMinionSpawned = false;
}

void UACMonsterManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);


	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		// MinionSpawnData
		if (true == DataManager->IsMinionSpawnDataValid())
		{
			TArray<FACMinionSpawnData> MinionSpawnDataArray = DataManager->GetMinionSpawnDataArray();

			for (FACMinionSpawnData MinionSpawnDataRow : MinionSpawnDataArray)
			{
				MaxMinionNum.Add(MinionSpawnDataRow.GamePhase, MinionSpawnDataRow.MaxSpawnCount);
				SpawnedMinionNum.Add(MinionSpawnDataRow.GamePhase, 0);
			}
		}
	}
}

void UACMonsterManager::AddMinionMonster(AACCharacterMinion* Minion)
{
	MinionArray.Add(Minion);

	if (AACGameState* GS = Cast< AACGameState>(GetWorld()->GetAuthGameMode()->GameState))
	{
		++SpawnedMinionNum[GS->GetCurrentGamePhase()];

		if (SpawnedMinionNum[GS->GetCurrentGamePhase()] == MaxMinionNum[GS->GetCurrentGamePhase()])
		{
			bIsAllMinionSpawned = true;
		}
	}
}

void UACMonsterManager::SetBossMonster(AACCharacterBoss* Boss)
{
	MonsterBoss = Boss;
}

void UACMonsterManager::DestroyBossMonster()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (APlayerController* PC = Cast<APlayerController>(Iterator->Get()))
		{
			if (AACCharacterHUD* HUD = Cast<AACCharacterHUD>(PC->GetHUD())) 
			{
				HUD->RemoveBossHPBarWidget();
			}
		}
	}
}

void UACMonsterManager::SetupHUDWidget(AACCharacterHUD* InHUDWidget)
{
	if (InHUDWidget) 
	{
		if (MonsterBoss) 
		{
			MonsterBoss->SetupHUDWidget(InHUDWidget);
		}
	}
}

void UACMonsterManager::RemoveMinionFromArray(class AActor* Minion)
{
	MinionArray.Remove(Cast<AACCharacterMinion>(Minion));

	if (MinionArray.Num() == 0 && bIsAllMinionSpawned== true)
	{
		if (OnAllMonsterDead.IsBound()) 
		{
			OnAllMonsterDead.Broadcast();
		}
		bIsAllMinionSpawned = false;
	}
}
