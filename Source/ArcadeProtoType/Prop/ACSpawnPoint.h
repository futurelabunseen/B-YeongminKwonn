#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/GamePhase/ACGamePhase.h"
#include "ACSpawnPoint.generated.h"

UENUM()
enum class ESpawnedCharacterType : uint8
{
	Player,
	Minion,
	Boss,
};

UCLASS()
class ARCADEPROTOTYPE_API AACSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AACSpawnPoint();
	FORCEINLINE const ESpawnedCharacterType GetSpawnedCharacterType() const { return SpawnedCharacter; }
	FORCEINLINE const EACGamePhaseList GetCombatType() const { return CombatType; }

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="CharacterType")
	ESpawnedCharacterType SpawnedCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CombatType")
	EACGamePhaseList CombatType;
};
