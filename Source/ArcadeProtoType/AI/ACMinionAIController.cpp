#include "AI/ACMinionAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ArcadeProtoType.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ACGameState.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Character/ACCharacterNonPlayer.h"
#include "Character/ACCharacterPlayer.h"

AACMinionAIController::AACMinionAIController()
{
	// BlackBoard
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Arcade/Blueprint/AI/BB_ACMinion.BB_ACMinion'"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	// BehaviorTree
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Arcade/Blueprint/AI/BT_ACMinion.BT_ACMinion'"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	// AIPerception
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	// SightConfig
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	AIPerceptionComponent->ConfigureSense(*SightConfig);
}

void AACMinionAIController::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AACMinionAIController::PerceptionUpdated);

}

void AACMinionAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool ReunResult = RunBehaviorTree(BTAsset);
		ensure(ReunResult);
	}
}

void AACMinionAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

AACCharacterPlayer* AACMinionAIController::GetTarget() const
{
	return Target;
}

void AACMinionAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}



void AACMinionAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	float MinDistance = TNumericLimits<float>::Max();

	for (AActor* OtherActor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerceptionComponent->GetActorsPerception(OtherActor, Info);

		if (Info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			if (OtherActor == nullptr)
				return;

			if (AACCharacterPlayer* Player = Cast<AACCharacterPlayer>(OtherActor))
			{
				// 가까운 액터 찾는다. 없으면 이전 액터
				float Distance = FVector::Dist(OtherActor->GetActorLocation(), GetPawn()->GetActorLocation());

				if (MinDistance > Distance)
				{
					MinDistance = Distance;
					Target = Cast<AACCharacterPlayer>(OtherActor);
				}

				if (Target)
				{
					Cast<AACCharacterNonPlayer>(GetPawn())->SetTarget(Target);
					MinionMode = ENpcState::Chase;
				}
			}
			else
			{
				MinionMode = ENpcState::Roaming;
			}
		}
	}
}

void AACMinionAIController::ProcessSights()
{
	// UE_LOG(LogTemp, Log, TEXT("ProcessSights "));
}

void AACMinionAIController::ProcessHearing()
{
	UE_LOG(LogTemp, Log, TEXT("ProcessHearing "));
}
