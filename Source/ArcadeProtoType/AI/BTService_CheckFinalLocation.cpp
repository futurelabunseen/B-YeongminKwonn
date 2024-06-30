
#include "AI/BTService_CheckFinalLocation.h"
#include "ACAI.h"
#include "AIController.h"

UBTService_CheckFinalLocation::UBTService_CheckFinalLocation()
{
	//노드 이름
	NodeName = TEXT("CheckFinalLocation");
	// 인터벌 설정
	Interval = 0.04f;
}

void UBTService_CheckFinalLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if (nullptr == ControllingPawn)
	{
		return;
	}

	IACNPCInterface* NPC = Cast<IACNPCInterface>(ControllingPawn);
	if (nullptr == NPC)
	{
		return;
	}

	const float DestinationTolerance = NPC->GetDestinationTolerance_InRoaming();
	FVector MoveToLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_MOVETOLOCATION);
	
	const float DistanceToDestination = FVector::Distance(ControllingPawn->GetActorLocation(), MoveToLocation);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_DISTANCETOFINAL, DistanceToDestination);

	if (DistanceToDestination <= DestinationTolerance) 
	{
		if (NPC->OnFlyingMoveFinished.IsBound()) 
		{
			NPC->OnFlyingMoveFinished.Broadcast(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
