
#include "AI/BTTask_FlyingMoveTo.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Character/ACCharacterNonPlayer.h"


UBTTask_FlyingMoveTo::UBTTask_FlyingMoveTo()
{
	NodeName = TEXT("FlyingMoveTo");
	bIsFaceTarget = true;
	bTickIntervals = 0.4f;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FlyingMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		NPC->OnFlyingMoveFinished.AddUObject(this, &UBTTask_FlyingMoveTo::FinishLatentTask);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_FlyingMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory,DeltaSeconds);

	FVector MoveToLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_MOVETOLOCATION);

	if (nullptr == ControllingPawn)
	{
		return;
	}

	CharacterMovementComponent = Cast<UCharacterMovementComponent>(Cast<ACharacter>(ControllingPawn)->GetCharacterMovement());

	// Update Velocity
	const FVector Direction = (MoveToLocation - ControllingPawn->GetActorLocation()).GetSafeNormal();

	CharacterMovementComponent->Velocity = Direction * DefaultSpeed;
}
