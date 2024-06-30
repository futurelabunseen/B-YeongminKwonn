#include "AI/BTDecorator_NearDestination.h"
#include "ACAI.h"
#include "AIController.h"
UBTDecorator_NearDestination::UBTDecorator_NearDestination()
{
	NodeName = TEXT("NearDestination");
	bTickIntervals = 0.04f;
}

bool UBTDecorator_NearDestination::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IACNPCInterface* NPC = Cast<IACNPCInterface>(ControllingPawn);
	if (nullptr == NPC)
	{
		return false;
	}

	const float DestinationTolerance = NPC->GetDestinationTolerance_InRoaming();
	const FVector MoveToLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_MOVETOLOCATION);
	
	// Reached Target Point
	const float DistanceToDestination = FVector::Distance(ControllingPawn->GetActorLocation(), MoveToLocation); //OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_DISTANCETOFINAL);  //
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_DISTANCETOFINAL, DistanceToDestination); //  //
	bResult = (DistanceToDestination <= DestinationTolerance);

	return bResult;
}
