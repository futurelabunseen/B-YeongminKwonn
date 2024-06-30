#include "Weapon/Projectile/ACProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/ACDataManager.h"
#include "ArcadeProtoType.h"
#include "Net/UnrealNetwork.h"
AACProjectileBase::AACProjectileBase()
{
	// PrimaryActorTick.bCanEverTick = true;
	// Collision Component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	SphereComponent->InitSphereRadius(50.f);
	SphereComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereComponent->CanCharacterStepUpOn = ECB_No;
	SetRootComponent(SphereComponent);

	// Static Mesh
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMeshAsset.Object) 
	{
		ProjectileMesh->SetStaticMesh(SphereMeshAsset.Object);
	}
	ProjectileMesh->SetupAttachment(SphereComponent);

	// Projectile Movement Component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = false;

	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->Bounciness = 10.f;

	// 
	ProjectileType = EProjectileType::eLiquidBall;

	// Network Setting
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	NetUpdateFrequency = 100.0f;
}

void AACProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACProjectileBase, SphereComponent);//ProjectileMesh
	DOREPLIFETIME(AACProjectileBase, ProjectileMesh);//
}

void AACProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->ProjectileGravityScale = 0.2f;
}

void AACProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		FACProjectileData ProjectileData = DataManager->GetProjectileData(ProjectileType);

		// Projectile Mesh
		ProjectileMesh->SetStaticMesh(ProjectileData.ProjectileMesh);
		ProjectileMesh->SetMaterial(0, ProjectileData.ProjectileMaterial);

		ProjectileMesh->SetupAttachment(SphereComponent);
		ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);

		// NiagaraSystem
		NaiagaraSystem = ProjectileData.NiagaraSystem;

		// projectileMovement
		ProjectileMovementComponent->InitialSpeed = ProjectileData.InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = ProjectileData.MaxSpeed;
		ProjectileMovementComponent->Bounciness = ProjectileData.Bounciness;
	}
}


void AACProjectileBase::LaunchToLocation(const FVector& StartLocation, const FVector& TargetLocation)
{
	if (ProjectileMovementComponent) 
	{
		ProjectileMovementComponent->Velocity = (TargetLocation - StartLocation).GetSafeNormal() * (ProjectileMovementComponent->InitialSpeed);
	}
}


void AACProjectileBase::SetProjectileSpeed(float InProjectileSpeed)
{
	ProjectileMovementComponent->InitialSpeed = InProjectileSpeed;
}
