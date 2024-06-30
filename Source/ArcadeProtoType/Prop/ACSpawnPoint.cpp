#include "ACSpawnPoint.h"

AACSpawnPoint::AACSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnedCharacter = ESpawnedCharacterType::Player;
}

