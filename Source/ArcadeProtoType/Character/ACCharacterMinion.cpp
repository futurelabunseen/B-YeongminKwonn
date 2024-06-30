
#include "Character/ACCharacterMinion.h"
#include "ACCharacterPlayer.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ACPlayerController.h"
#include "EngineUtils.h"
#include "ArcadeProtoType.h"
#include "Animation/ACMinionAnimInstance.h"
#include "Character/ACCharacterPlayer.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/NPCAttack/ACMinionAttackComponent.h"

AACCharacterMinion::AACCharacterMinion()
{
	// Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Ancient_Golem/Mesh/SK_Ancient_Golem_Moving_Wheels.SK_Ancient_Golem_Moving_Wheels'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Arcade/Blueprint/ABP_ACMinion.ABP_ACMinion_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// DeadEffect Material
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstanceRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Arcade/Material/MI_DeadMinionMaterial.MI_DeadMinionMaterial'"));
	if (MaterialInstanceRef.Succeeded())
	{
		DeadMaterialInstance = MaterialInstanceRef.Object;
	}

	MinionAttackComponent = CreateDefaultSubobject<UACMinionAttackComponent>(TEXT("AttackComponent"));
	MinionAttackComponent->SetIsReplicated(true);
	LastTargetIndex = 0;

	MonsterType = EMonsterType::eMinion;
}

float AACCharacterMinion::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetDead();
	return DamageAmount;
}

void AACCharacterMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead) 
	{
		UpdateMaterialDissolve(DeltaTime);
	}
}




void AACCharacterMinion::BeginPlay()
{
	Super::BeginPlay();

	// GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AACCharacterMinion::OnHit);
	
	if (UACMinionAnimInstance* AnimInstance = Cast< UACMinionAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->OnStartAttackEvent.AddUObject(this, &AACCharacterMinion::BeginAttack);
		AnimInstance->OnEndAttackEvent.AddUObject(this, &AACCharacterMinion::EndAttack);
	}

	// Test
	// FTimerHandle timerHandle;
	// GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
	// 	{
	// 		Destroy();
	// 	}
	// ), 20.f, false, 20.f);
}

void AACCharacterMinion::SetDead()
{
	Super::SetDead();

	EndAttack();
}

void AACCharacterMinion::AttackByAI()
{
	if (nullptr == Target) 
	{
		return;
	}
	if ( true == Target->IsDead())
	{
		return;
	}
	else 
	{
		MinionAttackComponent->ActivateAttack(true);
	}
}

void AACCharacterMinion::BeginAttack()
{
	GetCapsuleComponent()->SetCapsuleRadius(80.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(80.f);
}

void AACCharacterMinion::EndAttack()
{
	MinionAttackComponent->ActivateAttack(false);
	OnAttackFinished.ExecuteIfBound();
	GetCapsuleComponent()->SetCapsuleRadius(40.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(40.f);
}

float AACCharacterMinion::GetAITurnSpeed()
{
	return 2.0f;
}

// 로밍 상태일 때 도착했다고 체크하는 허용 범위
float AACCharacterMinion::GetDestinationTolerance_InRoaming()
{
	return 200.f;
}

// 타겟을 감지할 수 있는 거리 -> Sight
float AACCharacterMinion::GetDetectRadius()
{
	return 500.f;
}

// 패트롤 상태일 때, 더 이상 타겟에게 가까이 오지 않는다고 설정한 범위
float AACCharacterMinion::GetDestinationTolerance_InPatrol()
{
	return 500.f;
}

// AttackRange
float AACCharacterMinion::GetAIAttackRange()
{
	return 1000.f;
}

void AACCharacterMinion::SetAIAttackDelegate(const FNPCAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

/*
void AACCharacterMinion::AttackPlayer()
{
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// BP : if 문 걸고
	// AttackTarget = Target;
	ensure(AttackTarget);
	const FVector ActorLocation = AttackTarget->GetActorLocation();
	FVector Direction = ActorLocation - GetActorLocation();
	Direction.Normalize();

	GetCharacterMovement()->Velocity = Direction;
}

void AACCharacterMinion::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (false == HasAuthority())
	{
		return;
	}

	if (OtherActor != this)
	{
		AC_LOG(LogACNetwork, Log, TEXT("%s"), *OtherActor->GetName());
		if (Cast<AACCharacterPlayer>(OtherActor))
		{
			AC_LOG(LogACNetwork, Log, TEXT("%s"), *OtherActor->GetName());

			FDamageEvent DamageEvents;
			const float AttackDamage = 30.f;
			AttackTarget->TakeDamage(AttackDamage, DamageEvents, this->GetController(), this);
		}
	}
}
*/
