#include "AI/BTDecorator_AttackInRange.h"
#include "ACAI.h"
#include "AIController.h"


UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	IACNPCInterface* NPC = Cast<IACNPCInterface>(ControllingPawn);
	if (nullptr == NPC)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	const float AttackRangeWithRadius = NPC->GetAIAttackRange();

	bResult = (DistanceToTarget <= AttackRangeWithRadius);

	// if(bResult)
	// {
	// 	DrawDebugSphere(GetWorld(), ControllingPawn->GetActorLocation(), AttackRangeWithRadius, 16, FColor::Red, true);
	// }
	// else 
	// {
	// 	DrawDebugSphere(GetWorld(), ControllingPawn->GetActorLocation(), AttackRangeWithRadius, 16, FColor::White, true);
	// }

	return bResult;
}