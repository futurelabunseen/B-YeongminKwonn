
#include "Weapon/Projectile/ACProjectileLiquidBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Net/UnrealNetwork.h"
#include "Character/MyACCharacterMonsterTemp.h"
#include "Character/ACCharacterMonster.h"
#include "Character/ACCharacterMinion.h"
#include "ArcadeProtoType.h"
#include "Character/ACCharacterNonPlayer.h"
#include "Weapon/ACTreeSapWeapon.h"
#include "EngineUtils.h"

AACProjectileLiquidBall::AACProjectileLiquidBall()
{
	// @zeniz
	PrimaryActorTick.bCanEverTick = true;

	// Stuck Material
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> StuckMaterialInstanceRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Arcade/Weapons/Projectiles/NiagaraSystem/Materials/MI_LiquidBall_Stuck.MI_LiquidBall_Stuck'"));
	if (StuckMaterialInstanceRef.Object)
	{
		StuckMaterial = StuckMaterialInstanceRef.Object;
	}
	ProjectileType = EProjectileType::eLiquidBall;
	LiquidTraceEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TreesapProjectileEffect"));
	SphereComponent->BodyInstance.SetCollisionProfileName(TEXT("LiquidProjectile"));
	CurrentLiquidBallState = ELiquidBallState::FiredState;
	SphereComponent->InitSphereRadius(50.f);
	SphereComponent->SetSimulatePhysics(false);

	NPCTarget = nullptr;
}


void AACProjectileLiquidBall::LaunchToLocation(const FVector& StartLocation, const FVector& TargetLocation)
{
	Super::LaunchToLocation(StartLocation, TargetLocation);
}


void AACProjectileLiquidBall::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (nullptr == OtherActor)
	{
		return;
	}

	if (OtherActor != this)
	{
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::KeepWorld,
			false);

		if (AACCharacterNonPlayer* NPC = Cast<AACCharacterNonPlayer>(OtherActor))
		{
			NPCTarget = NPC;
			bIsTarget = true;
			
			ResetProjectileMovement();

			if (NPCTarget) 
			{
				NPCTarget->GetMesh()->IgnoreActorWhenMoving(this, true);
				NPCTarget->MoveIgnoreActorAdd(this);

				NPCTarget->AddStuckProjectileArray(this);
				AttachToComponent(NPCTarget->GetMesh(), AttachmentRules);
			}
		}
	}

	if (AACProjectileLiquidBall* LiquidProjectile = Cast<AACProjectileLiquidBall>(OtherActor))
	{
		if (CurrentLiquidBallState == ELiquidBallState::StuckState)
		{
			ScaleUp(OtherActor->GetActorScale3D());
			OtherActor->Destroy();
		}
		else
		{
			if (Cast<AACProjectileLiquidBall>(OtherActor)->GetLiquidBallState() == ELiquidBallState::FiredState)
			{
				SphereComponent->SetSimulatePhysics(true);
				CurrentLiquidBallState = ELiquidBallState::StuckState;

				if (OtherActor->GetActorScale3D().Size() < GetActorScale3D().Size())
				{
					OtherActor->Destroy();
					ScaleUp(OtherActor->GetActorScale3D());
				}
			}
		}
	}
	else
	{
		CurrentLiquidBallState = ELiquidBallState::StuckState;
		ProjectileMesh->SetMaterial(0, StuckMaterial);
		SphereComponent->SetSimulatePhysics(false);
	}
}

void AACProjectileLiquidBall::SetSimulatePhysics(bool bSet)
{
	SphereComponent->SetSimulatePhysics(bSet);
}

void AACProjectileLiquidBall::ScaleUp(FVector OtherActorScale)
{
	const FVector NewScale = GetActorScale3D() + OtherActorScale;
	const float ScaleOffset = 0.85f;
	SetActorScale3D(NewScale * ScaleOffset);
}

bool AACProjectileLiquidBall::IsTarget() const
{
	return bIsTarget;
}

AACCharacterNonPlayer* AACProjectileLiquidBall::GetTarget() const
{
	return NPCTarget;
}

void AACProjectileLiquidBall::BeginPlay()
{
	Super::BeginPlay();
	
	if (NaiagaraSystem) 
	{
		LiquidTraceEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(NaiagaraSystem, SphereComponent, FName(TEXT("TraceComponent")), FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::Type::KeepRelativeOffset, true, true);
	}

	if (HasAuthority()) 
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AACProjectileLiquidBall::OnActorBeginOverlap);
	}
}

void AACProjectileLiquidBall::ResetProjectileMovement()
{
	ProjectileMovementComponent->Velocity = FVector(0.f, 0.f, 0.f);
	ProjectileMovementComponent->InitialSpeed = 0.f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->SetActive(false);
}

void AACProjectileLiquidBall::OnRep_UpdateLiquidState() 
{
	ProjectileMesh->SetMaterial(0, StuckMaterial);
}

void AACProjectileLiquidBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACProjectileLiquidBall, CurrentLiquidBallState); 
	DOREPLIFETIME(AACProjectileLiquidBall, SphereComponent); 
	DOREPLIFETIME(AACProjectileLiquidBall, ProjectileMesh);
	DOREPLIFETIME(AACProjectileLiquidBall, bIsTarget);
}
