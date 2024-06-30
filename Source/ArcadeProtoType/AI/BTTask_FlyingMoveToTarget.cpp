
#include "AI/BTTask_FlyingMoveToTarget.h"
#include "ACAI.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

UBTTask_FlyingMoveToTarget::UBTTask_FlyingMoveToTarget()
{
	NodeName = TEXT("FlyingMoveToTarget");
	bTickIntervals = 0.4f;
	bNotifyTick = true;
	DefaultSpeed = 300.f;
}

EBTNodeResult::Type UBTTask_FlyingMoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	CharacterMovementComponent = Cast<UCharacterMovementComponent>(Cast<ACharacter>(ControllingPawn)->GetCharacterMovement());

	IACNPCInterface* NPC = Cast<IACNPCInterface>(ControllingPawn);
	if (nullptr == NPC)
	{
		return EBTNodeResult::Failed;
	}
	if (false == NPC->OnFlyingMoveFinished.IsBoundToObject(this))
	{
		NPC->OnFlyingMoveFinished.AddUObject(this, &UBTTask_FlyingMoveToTarget::FinishLatentTask);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_FlyingMoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	bool bStop = false;
	if (true == bStop) 
	{
		return;
	}

	IACNPCInterface* NPC = Cast<IACNPCInterface>(ControllingPawn);
	if (nullptr == NPC)
	{
		return;
	}

	const float DestinationTolerance = NPC->GetDestinationTolerance_InPatrol();
	FVector MoveToLocation = FVector(0.f, 0.f, 0.f);
	if (APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET))) 
	{
		MoveToLocation = Target->GetActorLocation();

	}
	MoveToLocation.Z = ControllingPawn->GetActorLocation().Z;

	float Distance = FVector::Distance(ControllingPawn->GetActorLocation(), MoveToLocation);

	FVector Direction = (MoveToLocation - ControllingPawn->GetActorLocation()).GetSafeNormal();

	if (Distance > DestinationTolerance) 
	{
		CharacterMovementComponent->Velocity = Direction * MaxSpeed;
	}
	else 
	{
		const float Radius = DestinationTolerance;
		float DeltaAngle = DefaultSpeed / Radius * DeltaSeconds;
		Angle += DeltaAngle;

		// 목표 위치 계산
		FVector Offset(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f);
		FVector TargetLocation = MoveToLocation + Offset;

		// 속도 벡터 계산
		FVector Velocity = (TargetLocation - ControllingPawn->GetActorLocation()).GetSafeNormal() * DefaultSpeed;
		CharacterMovementComponent->Velocity = Velocity;
	}
}
