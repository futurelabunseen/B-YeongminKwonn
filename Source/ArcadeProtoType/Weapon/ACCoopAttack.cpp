#include "Weapon/ACCoopAttack.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/ACDataManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "ArcadeProtoType.h"
#include "Character/ACCharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "Character/ACCharacterNonPlayer.h"
#include "Engine/DamageEvents.h"

AACCoopAttack::AACCoopAttack()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	SphereComponent->InitSphereRadius(50.f);
	SphereComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereComponent->CanCharacterStepUpOn = ECB_No;
	SetRootComponent(SphereComponent);

	AttackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	AttackMesh->SetupAttachment(SphereComponent);
	AttackMesh->SetVisibility(false);

	ProjectileType = EProjectileType::eCoopAttack;
}

void AACCoopAttack::BeginPlay()
{
	Super::BeginPlay();
	ExplosionEffect = UGameplayStatics::SpawnEmitterAttached(ParticleSystem, SphereComponent,FName(TEXT("SphereComponent")), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease,true);
	
	ExplosionEffect->SetRelativeScale3D(Scale);
	SphereComponent->OnComponentHit.AddDynamic(this, &AACCoopAttack::OnHit);
}


void AACCoopAttack::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		FACProjectileData ProjectileData = DataManager->GetProjectileData(ProjectileType);

			if (ProjectileData.ProjectileMesh)
			{
				AttackMesh->SetStaticMesh(ProjectileData.ProjectileMesh);
			}
			// LiquidBall Mesh
			AttackMesh->SetupAttachment(SphereComponent);

			// NiagaraSystem
			ParticleSystem = ProjectileData.ParticleSystem;
	}

}

void AACCoopAttack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this) 
	{
		// Destroy();
	}
}

void AACCoopAttack::SetScale(FVector NewScale)
{
	Scale = NewScale;
}

