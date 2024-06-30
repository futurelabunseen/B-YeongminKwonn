
#include "Weapon/Projectile/ACProjectileFireBall.h"
#include "Weapon/Projectile/ACProjectileLiquidBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "ArcadeProtoType.h"
#include "EngineUtils.h"
#include "Character/ACCharacterNonPlayer.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Manager/ACMonsterManager.h"

AACProjectileFireBall::AACProjectileFireBall()
{
	FireBallTraceEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TreesapProjectileEffect"));

	SphereComponent->BodyInstance.SetCollisionProfileName(TEXT("FireProjectile"));
	ProjectileType = EProjectileType::eFireBall;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleClassRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B'"));
	if (ParticleClassRef.Object) 
	{
		ExplosionParticleEffect = ParticleClassRef.Object;
	}
}

void AACProjectileFireBall::BeginPlay()
{
	Super::BeginPlay();
	
	// if (FireBallTraceEffect) 
	// {
	// 	FireBallTraceEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(NaiagaraSystem, SphereComponent, FName(TEXT("TraceComponent")), FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::Type::KeepRelativeOffset, true, true);
	// 	AC_LOG(LogACNetwork, Log, TEXT("FireBallTraceEffect"));
	// }

	MulticastSpawnNiagaraEffect();
	SphereComponent->OnComponentHit.AddDynamic(this, &AACProjectileFireBall::OnHit);

	ProjectileMovementComponent->InitialSpeed = 5000.f;
	ProjectileMovementComponent->MaxSpeed = 6000.f;
}



void AACProjectileFireBall::MulticastSpawnNiagaraEffect_Implementation() 
{
	if (FireBallTraceEffect)
	{
		FireBallTraceEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(NaiagaraSystem, SphereComponent, FName(TEXT("TraceComponent")), FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::Type::KeepRelativeOffset, true, true);
		AC_LOG(LogACNetwork, Log, TEXT("FireBallTraceEffect"));
	}
}
void AACProjectileFireBall::LaunchToLocation(const FVector& StartLocation, const FVector& TargetLocation)
{
	Super::LaunchToLocation(StartLocation, TargetLocation);
}

void AACProjectileFireBall::Launch(FVector ProjectileDirection)
{
	ProjectileMovementComponent->Velocity = ProjectileDirection * ProjectileMovementComponent->InitialSpeed;
}

void AACProjectileFireBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (AACProjectileLiquidBall* LiquidBall = Cast<AACProjectileLiquidBall>(OtherActor))
		{
			if (ELiquidBallState::StuckState == LiquidBall->GetLiquidBallState())
			{
				if (AACCharacterNonPlayer* NPC = LiquidBall->GetTarget())
				{
					const float AttackDamage = 20.f;
					FDamageEvent DamageEvents;
					NPC->TakeDamage(AttackDamage, DamageEvents, Cast<APawn>(GetOwner())->GetController(), this);

					if (NPC->GetMonsterType() == EMonsterType::eMinion) 
					{
						if (UACMonsterManager* MonsterManager = GetWorld()->GetSubsystem<UACMonsterManager>())
						{
							MonsterManager->RemoveMinionFromArray(NPC);
						}
					}
				}
				
				const FVector ExplosionEffectScale = LiquidBall->GetActorScale3D();
				
				if (true == HasAuthority())
				{
					// 서버에 멀티캐스트 RPC 요청한다.
					MulticastRPCActionCoopAttack(ExplosionEffectScale);
					// 서버에서 ActionCoopAttack 함수를 호출한다.
					ActionCoopAttack(ExplosionEffectScale);
					// LiquidBall과 자신을 파괴한다.
					LiquidBall->Destroy();
					Destroy();
				}
				
			}
		}
		else 
		{
			Destroy();
		}
	}
}

void AACProjectileFireBall::ActionCoopAttack(FVector Scale)
{
	UParticleSystemComponent* EffectParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ExplosionParticleEffect,GetTransform(), true, EPSCPoolMethod::AutoRelease, true);
	EffectParticle->SetRelativeScale3D(Scale);
}

void AACProjectileFireBall::ServerRPCActionCoopAttack_Implementation(FVector Scale) 
{
	ActionCoopAttack(Scale);
}

void AACProjectileFireBall::MulticastRPCActionCoopAttack_Implementation(FVector Scale)
{
	ActionCoopAttack(Scale);
}
