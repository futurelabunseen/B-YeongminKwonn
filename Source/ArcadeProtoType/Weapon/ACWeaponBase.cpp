
#include "Weapon/ACWeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Character/ACCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/ACDataManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ArcadeProtoType.h"
#include "Net/UnrealNetwork.h"
#include "Projectile/ACProjectileBase.h"
#include "HUD/ACCharacterHUD.h"

AACWeaponBase::AACWeaponBase()
{
	// SceneComponent
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneComponent"));
	if (DefaultSceneComponent) 
	{
		SetRootComponent(DefaultSceneComponent);
	}
	 
	// CollisionComponent

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("CollisionComponent"));
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->InitCapsuleSize(40.0f, 50.0f);

	//
	RootComponent = CollisionComponent;

	// Mesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponRelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	WeaponRotator = FRotator(0.0f, 0.0f, 0.0f);

	
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetupAttachment(CollisionComponent);
	WeaponMesh->SetRelativeLocation(WeaponRelativeLocation);
	WeaponMesh->CastShadow = true;
	// WeaponMesh->SetVisibility(true, true);
	WeaponMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;

	// Set this actor to never tick
	PrimaryActorTick.bCanEverTick = false;

	// Network Setting
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	NetUpdateFrequency = 100.0f; 

	// Initialize Weapon TypeServerRPCCreateProjectile 
	WeaponType = EWeaponType::eTreeSapWeapon;
}


void AACWeaponBase::ResetWeapon()
{
	SetAmmo(MaxAmmoCount);
}

void AACWeaponBase::SetActorHiddenInGame(bool bNewHidden)
{
	if (bNewHidden) 
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadAmmoTimer);
	}

	Super::SetActorHiddenInGame(bNewHidden);
}

void AACWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	if (AACCharacterHUD* HUD = GetWorld()->GetFirstPlayerController()->GetHUD<AACCharacterHUD>()) 
	{
		OnAmmoChanged.AddUObject(HUD, &AACCharacterHUD::UpdateAmmoWidget);
	}

	// OnAmmoZero.BindUObject(this, &AACWeaponBase::ReloadAmmo);
	ReloadAmmo();
}

void AACWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		FACWeaponData WeaponData = DataManager->GetWeaponData(WeaponType);

		MaxAmmoCount = WeaponData.MaxAmmoCount;
		CurrentAmmoCount = MaxAmmoCount;
		MaxShootRange = WeaponData.MaxShootRange;
		
	}
}


void AACWeaponBase::Equip()
{
	Character = Cast<AACCharacterPlayer>(GetOwner());

	if (nullptr == Character) 
	{
		return;
	}

	const FName AttachPoint = TEXT("WeaponSocketR");

	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		true);

	AttachToComponent(Character->GetMesh(), AttachmentRules, AttachPoint);

	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetGenerateOverlapEvents(false);

	// Ammo
	CurrentAmmoCount = MaxAmmoCount;
}

void AACWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACWeaponBase, Character);
	DOREPLIFETIME(AACWeaponBase, CurrentAmmoCount);
}


void AACWeaponBase::ReloadAmmo()
{
	const float ReloadTime = 10.f;
	ReloadAmmoOnTimer();
	// GetWorld()->GetTimerManager().SetTimer(ReloadAmmoTimer,this, &AACWeaponBase::ReloadAmmoOnTimer,ReloadTime, true);
}

void AACWeaponBase::ReloadAmmoOnTimer()
{
	const int32 AmmoTolerance = 4;
	const int32 NewAmmo = MaxAmmoCount / AmmoTolerance + CurrentAmmoCount;
	SetAmmo(NewAmmo);
}

// Shooting Functions -> Interface
// ========================= Fire =========================

bool AACWeaponBase::OnFire(bool bIsDead)
{
	if (false == HasAmmo())
	{
		ReloadAmmo();
		AC_LOG(LogACNetwork, Log, TEXT("CurrentAmmoCount : %d"), CurrentAmmoCount);
		return false;
	}
	return true;
}

void AACWeaponBase::EndFire()
{

}

void AACWeaponBase::CreateProjectile(FVector StartLocation, FRotator TargetRotation)
{

}

void AACWeaponBase::SetAmmo(int NewAmmoCount)
{
	if (NewAmmoCount < MaxAmmoCount)
	{
		CurrentAmmoCount = NewAmmoCount;
	}
	else 
	{
		CurrentAmmoCount = MaxAmmoCount;
	}
	
	if (OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(NewAmmoCount, MaxAmmoCount);
	}
}
