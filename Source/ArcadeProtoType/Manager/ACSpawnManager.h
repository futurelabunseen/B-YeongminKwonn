#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/ACDataUtility.h"
#include "Game/GamePhase/ACGamePhase.h"
#include "Prop/ACSpawnPoint.h"
#include "ACSpawnManager.generated.h"

/**
 * 
 */


UCLASS()
class ARCADEPROTOTYPE_API UACSpawnManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UACSpawnManager();
	void StartSpawn(EMonsterType MonsterType, EACGamePhaseList GamePhase);
	void DoSpawn(EMonsterType MonsterType, EACGamePhaseList GamePhase);
	void RequestRespawnCharacter(APawn* Pawn, EACGamePhaseList GamePhase);

	FORCEINLINE void SetStopSpawn(bool NewStopSpawned) { bStopSpawned = NewStopSpawned; }
	FORCEINLINE bool IsbStopSpawn() const { return bStopSpawned; }

protected:
	virtual void PostInitialize() override;
	virtual void SpawnBoss();
	virtual void SpawnMinion(EACGamePhaseList GamePhase);
	
private:
	int32 GetRandomMinonSpawnPointIndex(int32 CombatIndex) const;
	uint8 bStopSpawned : 1;

protected:
	FTimerHandle SpawnTimerHandle;

	TMap<EMonsterType, TSubclassOf<class AACCharacterNonPlayer>> MonsterClassMap;
	TMap<ECharacterType, UClass*> PlayerClassMap;
	TMap<EACGamePhaseList, FACMinionSpawnData> MinionSpawnData;
	TMap<EACGamePhaseList,class AACSpawnPoint*> PlayerRespawnPoint;
	
	TObjectPtr<class AACSpawnPoint> BossSpawnPoint;
	TArray<class AACSpawnPoint*> MinionSpawnPointArray[ACGamePhaseUtility::ENUMNUM];
	int32 CurrentMonsterSpawnCount[ACGamePhaseUtility::ENUMNUM];
};
