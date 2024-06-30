#include "Weapon/NPCAttack/ACMinionAttackComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "kismet/GameplayStatics.h"
#include "Character/ACCharacterNonPlayer.h"
#include "Character/ACCharacterPlayer.h"
#include "Engine/DamageEvents.h"
#include "ArcadeProtoType.h"
#include "Net/UnrealNetwork.h"


UACMinionAttackComponent::UACMinionAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Ring Blur Effect
	AttackEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TreesapProjectileEffect"));
	
	// AttackEffectNS
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/Arcade/Weapons/NPCAttack/NS_MinionAttack.NS_MinionAttack'"));
	if (AttackEffectRef.Object)
	{
		AttackEffectNS = AttackEffectRef.Object;
	}

	// Initialize
	HalfAttackAngle = 300.f;
	AttackInterval = 1.0f;
}


void UACMinionAttackComponent::BeginAttack()
{
	SpawnAttackEffect();
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UACMinionAttackComponent::OnAttack, AttackInterval, true);

	HalfAttackAngle = AttackRange / 2;
}

void UACMinionAttackComponent::OnAttack()
{
	// 인터페이스로 바꾸기
	AACCharacterNonPlayer* NPC = Cast< AACCharacterNonPlayer>(GetOwner());
	
	if (nullptr == NPC)
	{
		return;
	}

	FVector OwnerForward = NPC->GetActorForwardVector();

	APawn* Target = NPC->GetTarget();
	
	if (nullptr == Target) 
	{
		return;
	}

	if (true == IsAttackEnable(Target->GetActorLocation()))
	{
		ApplyDamage(Target);
	}
}

void UACMinionAttackComponent::SpawnAttackEffect()
{
	const FVector Location = FVector(0, 0, 0);
	const FRotator Rotator = FRotator(0, 0, 0);

	AttackEffectComponent =
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			AttackEffectNS,
			Cast<ACharacter>(GetOwner())->GetMesh(),
			AttachPoint,
			Location,
			Rotator,
			EAttachLocation::Type::KeepRelativeOffset,
			true,
			true);
}

void UACMinionAttackComponent::OnChangeAttackBegin()
{
	if (true == bIsActiveAttack) 
	{
		BeginAttack();
	}
	else 
	{
		AC_SUBLOG(LogACNetwork, Log, TEXT(""));
		EndAttack();
	}
}

void UACMinionAttackComponent::OnRep_BeginAttack()
{
	OnChangeAttackBegin();
}

void UACMinionAttackComponent::EndAttack()
{
	AttackEffectComponent->Deactivate();
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void UACMinionAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UACMinionAttackComponent, bIsActiveAttack);
}

void UACMinionAttackComponent::InitializeComponent()
{
	Super::InitializeComponent();
	AttackEffectComponent->SetIsReplicated(true);
}

void UACMinionAttackComponent::ActivateAttack(bool InBeginAttack)
{
	if (GetOwner()->HasAuthority()) 
	{
		bIsActiveAttack = InBeginAttack;
		OnChangeAttackBegin();
	}
}



bool UACMinionAttackComponent::IsAttackEnable(const FVector TargetLocation)
{
	if (GetOwner() == nullptr) 
	{
		return false;
	}
	
	const FVector MinionLocation = GetOwner()->GetActorLocation();
	const FVector MinionForwardVector = GetOwner()->GetActorForwardVector();

	FVector DirectionToActor = TargetLocation - MinionLocation;
	DirectionToActor.Normalize();

	float DistanceToActor = FVector::Dist(MinionLocation, TargetLocation);
	float AngleToActor = FMath::RadiansToDegrees(acosf(FVector::DotProduct(MinionForwardVector, DirectionToActor)));

	////////////////////////////////// Debug //////////////////////////////////
	// DrawDebugSphere(GetWorld(), MinionLocation, AttackRange, 12, FColor::Green, false, 1.0f);
	// DrawDebugCone(GetWorld(), MinionLocation, MinionForwardVector, AttackRange, FMath::DegreesToRadians(HalfAttackAngle), FMath::DegreesToRadians(HalfAttackAngle), 12, FColor::Blue, false, 1.0f);
	// DrawDebugLine(GetWorld(), MinionLocation, TargetLocation, FColor::Red, false, 1.0f, 0, 1.0f);
	/////////////////////////////////////////////////////////////////////////////

	return ((DistanceToActor <= AttackRange) && (AngleToActor <= HalfAttackAngle));
}

