
#include "Character/ACCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Stat/ACCharacterStatComponent.h"
#include "Animation/ACAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "ArcadeProtoType.h"

AACCharacterBase::AACCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Arcade/CharacterAssets/Meshes/SM_Character1.SM_Character1'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Arcade/Blueprint/ABP_ACCharacterPlayer.ABP_ACCharacterPlayer_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	CharacterType = ECharacterType::eMantus;

	// Stat Component
	Stat = CreateDefaultSubobject<UACCharacterStatComponent>(TEXT("Stat"));

	// DeadEffect Material
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstanceRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Arcade/Material/MI_DeadMinionMaterial.MI_DeadMinionMaterial'"));
	if (MaterialInstanceRef.Succeeded())
	{
		DeadMaterialInstance = MaterialInstanceRef.Object;
	}

	// 
}

void AACCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AACCharacterBase::SetDead);
}

void AACCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) 
	{
		bIsDead = false;
		OnChangeDeadState();
	}
}

float AACCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead == true) 
	{
		return DamageAmount;
	}
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);
	return DamageAmount;
}

void AACCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AACCharacterBase, Stat);
	DOREPLIFETIME(AACCharacterBase, bIsDead);
}

void AACCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority()) 
	{
		if (bIsDead)
		{
			UpdateMaterialDissolve(DeltaSeconds);
		}
	}
}

void AACCharacterBase::PlayDeadAnimation()
{
	if (UACAnimInstance* AnimInstance = Cast< UACAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->PlayDeadMontage();
	}
}

void AACCharacterBase::SetDead()
{
	if (HasAuthority()) 
	{
		bIsDead = true;
		OnChangeDeadState();
		// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetCharacterMovement()->DisableMovement();
		SetActorEnableCollision(false);
		ServerRPCPlayDeadAnimation();
	}
}

void AACCharacterBase::UpdateMaterialDissolve(float DeltaTime)
{
	const float InterpSpeed = 1.f;
	const float ChangeVlaue = -1.f;

	DissolveValue = FMath::FInterpTo(DissolveValue, ChangeVlaue, DeltaTime, InterpSpeed);

	if (DeadMaterialInstanceDynamic)
	{
		DeadMaterialInstanceDynamic->SetScalarParameterValue(FName("Dissolve"), DissolveValue);
		GetMesh()->SetMaterial(0, DeadMaterialInstanceDynamic);
	}

	if (DissolveValue <= ChangeVlaue)
	{
		Destroy();
	}
}

void AACCharacterBase::OnChangeDeadState()
{
	if (true == bIsDead)
	{
		DeadMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(DeadMaterialInstance, this);
		if (DeadMaterialInstanceDynamic)
		{
			for (int i = 0; i < 15; ++i)
			{
				GetMesh()->SetMaterial(i, DeadMaterialInstanceDynamic);
			}
		}
	}
	else
	{
		ResetMaterial();
	}
}

void AACCharacterBase::OnRep_Dead()
{
	OnChangeDeadState();
}

void AACCharacterBase::ResetMaterial()
{
}


void AACCharacterBase::MulticastPlayDeadAnimation_Implementation() 
{
	PlayDeadAnimation();
}

void AACCharacterBase::ServerRPCPlayDeadAnimation_Implementation()
{
	MulticastPlayDeadAnimation();
}

