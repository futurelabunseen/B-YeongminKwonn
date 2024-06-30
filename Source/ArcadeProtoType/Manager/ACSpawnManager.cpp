
#include "Manager/ACSpawnManager.h"
#include "Manager/ACMonsterManager.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"

#include "Character/ACCharacterMantus.h"
#include "Character/ACCharacterCollie.h"
#include "Character/ACCharacterMinion.h"
#include "Character/ACCharacterBoss.h"

#include "Manager/ACDataManager.h"
#include "Player/ACPlayerController.h"

UACSpawnManager::UACSpawnManager()
{
	// Mantus
	static ConstructorHelpers::FClassFinder<AACCharacterPlayer> MantusClassRef(TEXT("/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_CharacterMantus.BP_CharacterMantus_C'"));
	if (MantusClassRef.Class)
	{
		PlayerClassMap.Add(ECharacterType::eMantus, MantusClassRef.Class);
	}
	
	// Collie
	static ConstructorHelpers::FClassFinder<AACCharacterPlayer> CollieClassRef(TEXT("/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_CharacterCollie.BP_CharacterCollie_C'"));
	if (CollieClassRef.Class)
	{
		PlayerClassMap.Add(ECharacterType::eCollie, CollieClassRef.Class);
	}

	// Minion
	static ConstructorHelpers::FClassFinder<AACCharacterNonPlayer> MinionClassRef(TEXT("/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_ACMinion.BP_ACMinion_C'"));
	if (MinionClassRef.Class)
	{
		MonsterClassMap.Add(EMonsterType::eMinion, MinionClassRef.Class);
	}

	// Boss
	static ConstructorHelpers::FClassFinder<AACCharacterNonPlayer> BossClassRef(TEXT("/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_ACBoss.BP_ACBoss_C'"));
	if (BossClassRef.Class)
	{
		MonsterClassMap.Add(EMonsterType::eBoss, BossClassRef.Class);
	}

	bStopSpawned = false;
}

void UACSpawnManager::PostInitialize()
{
	Super::PostInitialize();

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		// MinionSpawnData
		if (true == DataManager->IsMinionSpawnDataValid())
		{
			TArray<FACMinionSpawnData> MinionSpawnDataArray = DataManager->GetMinionSpawnDataArray();

			for (FACMinionSpawnData MinionSpawnDataRow : MinionSpawnDataArray)
			{
				MinionSpawnData.Add(MinionSpawnDataRow.GamePhase, FACMinionSpawnData(MinionSpawnDataRow.GamePhase, MinionSpawnDataRow.SpawnRate, MinionSpawnDataRow.MaxSpawnCount));
			}
		}
	}

	// Initialize
	for (uint8 i = 0; i < (uint8)EACGamePhaseList::MAX; ++i)
	{
		CurrentMonsterSpawnCount[i] = 0;
	}

	if (AACSpawnPoint::StaticClass())
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(GetWorld(), EGetWorldErrorMode::LogAndReturnNull))
		{
			for (TActorIterator<AACSpawnPoint> It(World, AACSpawnPoint::StaticClass()); It; ++It)
			{
				AACSpawnPoint* SpawnPoint = *It;
				if (SpawnPoint)
				{
					// Minion SpawnPoint
					if (ESpawnedCharacterType::Minion == SpawnPoint->GetSpawnedCharacterType())
					{
						int32 CombatIndex = static_cast<int32>(SpawnPoint->GetCombatType()); //ACGamePhaseUtility::GamePhaseToIndex();
						if (CombatIndex < 0) 
						{
							return;
						}
						MinionSpawnPointArray[CombatIndex].Add(SpawnPoint);
					}

					// Boss SpawnPoint
					if (ESpawnedCharacterType::Boss == SpawnPoint->GetSpawnedCharacterType())
					{
						BossSpawnPoint = SpawnPoint;
					}

					// CharacterRespawnPoint -> 캐릭터의 리스폰 장소는 Minion
					if (ESpawnedCharacterType::Player == SpawnPoint->GetSpawnedCharacterType())
					{
						EACGamePhaseList CombatType = SpawnPoint->GetCombatType();
						PlayerRespawnPoint.Add(CombatType, SpawnPoint);
					}
				}
			}
		}
	}
}

void UACSpawnManager::SpawnBoss()
{
	if (bStopSpawned == true) 
	{
		return;
	}
	const FVector SpawnLocation = BossSpawnPoint->GetActorLocation();
	const FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation, FVector(1.f, 1.f, 1.f));

	if (AACCharacterNonPlayer* Monster = GetWorld()->SpawnActor<AACCharacterNonPlayer>(MonsterClassMap[EMonsterType::eBoss], SpawnTransform))
	{
		AACCharacterBoss* Boss = Cast<AACCharacterBoss>(Monster);

		if (UACMonsterManager* MonsterManager = GetWorld()->GetSubsystem<UACMonsterManager>())
		{
			MonsterManager->SetBossMonster(Boss);
		}
		
	}
}

void UACSpawnManager::SpawnMinion(EACGamePhaseList GamePhase)
{
	if (bStopSpawned == true)
	{
		return;
	}
	const int32 CombatIndex = ACGamePhaseUtility::GamePhaseToIndex(GamePhase);
	const int MaxSpawnCount = MinionSpawnData[GamePhase].MaxSpawnCount;
	if (MaxSpawnCount < CurrentMonsterSpawnCount[CombatIndex]+1)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	if (MinionSpawnPointArray[CombatIndex].Num() == 0 && MonsterClassMap.Num() == 0)
	{
		return;
	}

	const int SpawnPointIndex = GetRandomMinonSpawnPointIndex(CombatIndex);
	const FVector SpawnLocation = MinionSpawnPointArray[CombatIndex][SpawnPointIndex]->GetActorLocation();
	const FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation, FVector(1.f, 1.f, 1.f));

	if (AACCharacterNonPlayer* Monster = 
		GetWorld()->SpawnActor<AACCharacterNonPlayer>(MonsterClassMap[EMonsterType::eMinion], SpawnTransform))
	{
		if (UACMonsterManager* MonsterManager = GetWorld()->GetSubsystem<UACMonsterManager>())
		{
			MonsterManager->AddMinionMonster(Cast<AACCharacterMinion>(Monster));
			++CurrentMonsterSpawnCount[static_cast<int32>(GamePhase)];
		}
	}
}

void UACSpawnManager::StartSpawn(EMonsterType MonsterType, EACGamePhaseList GamePhase)
{
	const float SpawnRate = MinionSpawnData[GamePhase].SpawnRate;

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateLambda([this,MonsterType,  GamePhase]()
	{
		DoSpawn(MonsterType, GamePhase);
	}
	), SpawnRate, true, 1.0f);
}

void UACSpawnManager::DoSpawn(EMonsterType MonsterType, EACGamePhaseList GamePhase)
{
	if (bStopSpawned == true)
	{
		return;
	}
	FActorSpawnParameters SpawnParams;
	
	switch (MonsterType)
	{
	case EMonsterType::eMinion:
		SpawnMinion(GamePhase);
		break;
	case EMonsterType::eBoss:
		SpawnBoss();
		break;
	default:
		break;
	}
}

void UACSpawnManager::RequestRespawnCharacter(APawn* Pawn, EACGamePhaseList GamePhase)
{
	if (PlayerRespawnPoint.Find(GamePhase) != nullptr) 
	{
		FVector PlayerLocation = PlayerRespawnPoint[GamePhase]->GetActorLocation();
		Pawn->SetActorLocation(PlayerLocation);
	}
}

int32 UACSpawnManager::GetRandomMinonSpawnPointIndex(int32 CombatIndex) const
{
	const int RandomPointIndex = FMath::RandRange(0, MinionSpawnPointArray[CombatIndex].Num() - 1);
	return RandomPointIndex;
}

// int32 UACSpawnManager::GetRandomPlayerSpawnPointIndex() const
// {
// 	const int RandomPointIndex = FMath::RandRange(0, PlayerRespawnPointArray.Num() - 1);
// 	return RandomPointIndex;
// }
