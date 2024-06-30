
#include "AI/ACAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ACAI.h"

AACAIController::AACAIController()
{
	// static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Arcade/Blueprint/AI/BB_ACMonster.BB_ACMonster'"));
	// if (BBAssetRef.Object)
	// {
	// 	BBAsset = BBAssetRef.Object;
	// }
	// 
	// // static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Arcade/Blueprint/AI/BT_ACMonster.BT_ACMonster'"));
	// static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Arcade/Blueprint/AI/BT_ACMonsterTemp1.BT_ACMonsterTemp1'"));
	// if (BTAssetRef.Object)
	// {
	// 	BTAsset = BTAssetRef.Object;
	// }
}

void AACAIController::RunAI()
{
	/*UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool ReunResult = RunBehaviorTree(BTAsset);
		if (ReunResult) 
		{
			UE_LOG(LogTemp, Log, TEXT("Run Success"));
		}
		else 
		{
			UE_LOG(LogTemp, Log, TEXT("Run Fail"));
		}
		ensure(ReunResult);
	}*/
}

void AACAIController::StopAI()
{
	/*UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}*/
}

void AACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// RunAI();
}
