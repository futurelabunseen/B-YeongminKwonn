#include "Weapon/ACTreeSapWeapon.h"
#include "Components/CapsuleComponent.h"
#include "projectile/ACProjectileLiquidBall.h"
#include "Character/ACCharacterPlayer.h"
#include "Manager/ACDataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ArcadeProtoType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AACTreeSapWeapon::AACTreeSapWeapon()
{
	WeaponType = EWeaponType::eTreeSapWeapon;
}

void AACTreeSapWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		FACWeaponData  WeaponData = DataManager->GetWeaponData(WeaponType);
		LiquidProjectileClass = WeaponData.ProjectileClass;
		ShootInterval = WeaponData.ShootInterval;
	}
}

bool AACTreeSapWeapon::OnFire(bool bIsDead)
{
	bool Result = Super::OnFire(bIsDead);

	if (Character->IsDead() || false == Result || Character == nullptr || Character->GetController() == nullptr)// || false == Character->IsLocallyControlled())
	{
		AC_LOG(LogACNetwork, Log, TEXT(""));
		return false;
	}

	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FHitResult TraceHitResult;
		FVector Start = CrosshairWorldPosition;
		float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
		Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		const FVector End = Start + CrosshairWorldDirection * MaxShootRange;

		Result = GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_WorldDynamic);

		const FTransform StartTransform = GetMuzzleTransform();
		const FVector StartLocation = StartTransform.GetLocation();
		SeverRPCCreateProjectile(StartLocation, Character->GetCameraForwardDirection().Rotation());

		// LineTrace Logic
		// if (false == Result)
		// {
		// 	SeverRPCCreateProjectile(StartLocation, Character->GetCameraForwardDirection().Rotation());
		// }
		// 
		// const FVector HitTarget = TraceHitResult.ImpactPoint;
		// const FVector TargetLocation = HitTarget - StartTransform.GetLocation();
		// const FRotator TargetRotation = TargetLocation.Rotation();
		// 
		// AActor* Target = TraceHitResult.GetActor();
		// const FVector RelativeLocation = HitTarget;
		// const FVector LocalImpactNormal = UKismetMathLibrary::NegateVector(TraceHitResult.ImpactNormal);
		// 
		// SeverRPCCreateProjectile(StartLocation, TargetRotation);
		SetAmmo(CurrentAmmoCount - 1);
	}

	return Result;
}

bool AACTreeSapWeapon::SeverRPCCreateProjectile_Validate(FVector StartLocation, FRotator TargetRotation)
{
	return true;
}

void AACTreeSapWeapon::SeverRPCCreateProjectile_Implementation(FVector StartLocation, FRotator TargetRotation)
{
	CreateProjectile(StartLocation, TargetRotation);
}

void AACTreeSapWeapon::CreateProjectile(FVector StartLocation, FRotator TargetRotation)
{
	TargetRotation.Normalize();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();// Cast<APawn>(GetOwner());
	if (GetWorld())
	{
		AACProjectileLiquidBall* Projectile = GetWorld()->SpawnActor<AACProjectileLiquidBall>(LiquidProjectileClass, StartLocation, TargetRotation, SpawnParams);

		if (Projectile)
		{
			const float MinScale = 0.1f;
			const float MaxScale = 0.5f;

			const FVector RandomScale = FVector(FMath::FRandRange(MinScale, MaxScale));
			Projectile->SetOwner(GetOwner());
			Projectile->SetActorScale3D(RandomScale);
			// Projectile->ProjectileMovementComponent->Velocity = Projectile->GetActorForwardVector() * Projectile->ProjectileMovementComponent->InitialSpeed;
			//--CurrentAmmoCount;
		}
	}
}

