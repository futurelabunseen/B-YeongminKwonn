#include "AI/BTTask_FindLocation.h"
#include "AI/ACMinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet//KismetMathLibrary.h"
#include "Prop/ACFlyingPatrolPoint.h"


UBTTask_FindLocation::UBTTask_FindLocation()
{
	NodeName = TEXT("FindLocation");
}

EBTNodeResult::Type UBTTask_FindLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector MoveToLocationNear = GetRandomPatrolPoint();
	
	switch (NpcState)
	{
	case ENpcState::Patrol:
		MoveToLocationNear = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_SELF);
		break;
	case ENpcState::Roaming:
		MoveToLocationNear = GetRandomPatrolPoint();
		break;
	case ENpcState::Chase:
		if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET) == nullptr) 
		{
			NpcState = ENpcState::Roaming;
			MoveToLocationNear = GetRandomPatrolPoint();
		}
		else
		{
			MoveToLocationNear = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET))->GetActorLocation();
		}
		break;
	default:
		break;
	}

	FVector RandomLocation = UKismetMathLibrary::RandomUnitVector();
	RandomLocation.Z = 0.f;
	RandomLocation.Normalize();
	RandomLocation *= UKismetMathLibrary::RandomFloatInRange(MinXYDistanceFromTargetActor, MaxXYDistanceFromTargetActor);

	FVector MoveToLocation = MoveToLocationNear + FVector(0, 0, ExtraAddedElevationToFinalLocation);
	MoveToLocation += RandomLocation;
	
	// if (GetWorld()) 
	// {
	//	if (NpcState == ENpcState::Roaming) 
	//	{
	//		 DrawDebugLine(GetWorld(), ControllingPawn->GetActorLocation(), MoveToLocation, FColor::Orange, true);
	//		 DrawDebugSphere(GetWorld(), MoveToLocation, 20.f, 16, FColor::Orange, true);
	//	}
	//	else 
	//	{
	//		 DrawDebugLine(GetWorld(), ControllingPawn->GetActorLocation(), MoveToLocation, FColor::Green, true);
	//		 DrawDebugSphere(GetWorld(), MoveToLocation, 20.f, 16, FColor::Green, true);
	//	}
	// }

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_MOVETOLOCATION, MoveToLocation);
	return EBTNodeResult::Succeeded;
}

// 리팩토링하기
FVector UBTTask_FindLocation::GetRandomPatrolPoint()
{
	TArray<AActor*> PatrolPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AACFlyingPatrolPoint::StaticClass(), PatrolPoints);
	
	if (PatrolPoints.Num() <= 0) 
	{
		return FVector(0, 0, 0);
	}

	int PatrolPointIndex = UKismetMathLibrary::RandomIntegerInRange(0, PatrolPoints.Num() - 1);
	
	return PatrolPoints[PatrolPointIndex]->GetActorLocation();
}
