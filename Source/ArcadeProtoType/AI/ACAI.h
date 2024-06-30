#pragma once
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ACNPCInterface.h"



#define BBKEY_HOMEPOS TEXT("HomePos")
#define BBKEY_PATROLPOS TEXT("PatrolPos")
#define BBKEY_TARGET TEXT("Target")
#define BBKEY_MOVETOLOCATION TEXT("MoveToLocation")
#define BBKEY_SELF TEXT("SelfActor")
#define BBKEY_TELEPORT TEXT("TeleportLocation")
#define BBKEY_DISTANCETOFINAL TEXT("DistanceToFinal")


UENUM()
enum class ENpcState : uint8
{
	Patrol,
	Roaming,
	Chase
};
