#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACGamePhase.generated.h"


UENUM(BlueprintType)
enum class EACGamePhaseList : uint8
{
	WaitingToStart	,	// UMETA(DisplayName = "Waiting To Start"),
	PlayerEntrance	,	//UMETA(DisplayName = "Player Entrance"),
	MinionCombat	,	//UMETA(DisplayName = "Minion Combat"),
	FinalCombat		,	//UMETA(DisplayName = "Final Combat"),
	GameOver			,	//UMETA(DisplayName = "Game Over"),
	MAX					
};

namespace ACGamePhaseUtility
{
	static const int32 ENUMNUM = 5;

	static const int32 WaitingToStart = 0;
	static const int32 PlayerEntrance = 1;
	static const int32 MinionCombat = 2;
	static const int32 FinalCombat = 3;
	static const int32 GameOver = 4;

	static int32 GamePhaseToIndex(EACGamePhaseList GamePhase)
	{
		switch (GamePhase)
		{
		case EACGamePhaseList::WaitingToStart:
			return WaitingToStart;
		case EACGamePhaseList::PlayerEntrance:
			return PlayerEntrance;
		case EACGamePhaseList::MinionCombat:
			return MinionCombat;
		case EACGamePhaseList::FinalCombat:
			return FinalCombat;
		case EACGamePhaseList::GameOver:
			return GameOver;
		default:
			return -1;
		}
	}
};


UCLASS()
class ARCADEPROTOTYPE_API AACGamePhase : public AActor
{
	GENERATED_BODY()
	
public:
	AACGamePhase();
	virtual void OnBeginPhase();
	virtual void OnPlayPhase(float DeltaTime);
	virtual void OnEndPhase();
};
