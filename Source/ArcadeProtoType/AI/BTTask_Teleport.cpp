#include "AI/BTTask_Teleport.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "ACAI.h"
#include "Prop/ACFlyingPatrolPoint.h"

UBTTask_Teleport::UBTTask_Teleport()
{
	NodeName = TEXT("Teleport");
}

EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	TArray<AActor*> PatrolPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AACFlyingPatrolPoint::StaticClass(), PatrolPoints);

	if (PatrolPoints.Num() <= 0)
	{
		return Result;
	}

	// int PatrolPointIndex = UKismetMathLibrary::RandomIntegerInRange(0, PatrolPoints.Num() - 1);

	// return PatrolPoints[PatrolPointIndex]->GetActorLocation();

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_TELEPORT, PatrolPoints[0]->GetActorLocation());
	const FVector MoveToLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_TELEPORT);
	// DrawDebugSphere(GetWorld(), MoveToLocation, 30.f, 16, FColor::Green, true, -1.f);
	// DrawDebugLine(GetWorld(), ControllingPawn->GetActorLocation(), MoveToLocation, FColor::Green, true, -1.f);
	// ControllingPawn->SetActorLocation(MoveToLocation);

	return Result;
}
