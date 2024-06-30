
#include "AI/BTTask_TurnToTarget.h"
#include "AI/ACAIController.h"


UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
    NodeName = TEXT("TurnToTarget");

	NpcState = ENpcState::Roaming;
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}


	IACNPCInterface* AIPawn = Cast<IACNPCInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	const float TurnSpeed = AIPawn->GetAITurnSpeed();
	
	FVector LookVector = FVector(0.f, 0.f, 0.f);

	if (NpcState == ENpcState::Roaming) 
	{
		FVector MoveToLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_MOVETOLOCATION);
		LookVector = MoveToLocation - ControllingPawn->GetActorLocation();
	}
	else if(NpcState == ENpcState::Chase)
	{
		APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
		if (nullptr == TargetPawn)
		{
			return EBTNodeResult::Failed;
		}
		LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	}

	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	
	
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}

