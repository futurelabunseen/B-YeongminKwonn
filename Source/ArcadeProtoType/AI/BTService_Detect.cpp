
#include "AI/BTService_Detect.h"
#include "ACAI.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	//노드 이름
	NodeName = TEXT("Detect");
	// 인터벌 설정
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IACNPCInterface* AIPawn = Cast<IACNPCInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	APawn* Target = AIPawn->GetTarget();

	if (Target) 
	{
		// UE_LOG(LogTemp, Log, TEXT("[TargetName] Target : %s"), *Target->GetName());
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Target);
		return;
	}
	else
	{
		// UE_LOG(LogTemp, Log, TEXT("[TargetName] NoTarget"));
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
}

bool UBTService_Detect::CheckInRadialRange(APawn* ControllingPawn, AActor* Target, float RadialAngle)
{
	FVector FirstVector = ControllingPawn->GetActorForwardVector();
	FVector SecondVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();

	float DegreeBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FirstVector, SecondVector)));
	if (DegreeBetween <= RadialAngle / 2.0f)
	{
		return true;
	}
	return false;
}
