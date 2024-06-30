
#include "Weapon/ACMatchStickWeapon.h"
#include "Components/CapsuleComponent.h"
#include "projectile/ACProjectileFireBall.h"
#include "projectile/ACProjectileLiquidBall.h"

#include "Manager/ACDataManager.h"
#include "Kismet/GameplayStatics.h"//UKismetMathLibrary
#include "Kismet/KismetMathLibrary.h"//
#include "ArcadeProtoType.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

#include "Character/ACCharacterCollie.h"
#include "Data/ACDataUtility.h"

#include "Character/ACCharacterBase.h"
#include "Character/MyACCharacterMonsterTemp.h"


AACMatchStickWeapon::AACMatchStickWeapon()
{
	WeaponType = EWeaponType::eMatchStickWeapon;
	// bIsDetecting = false;

	PrimaryActorTick.bCanEverTick = false;
}


void AACMatchStickWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		FACWeaponData  WeaponData = DataManager->GetWeaponData(WeaponType);
		FireBallProjectileClass = WeaponData.ProjectileClass;
		MaxShootRange = WeaponData.MaxShootRange;
	}
}


bool AACMatchStickWeapon::IsDetecting()
{
	if (Character == nullptr)
	{
		return false;
	}

	if (false == Character->IsAiming())
	{
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

		if (GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility)) 
		{
			if (Cast<AACProjectileLiquidBall>(TraceHitResult.GetActor())) 
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

bool AACMatchStickWeapon::SeverRPCCreateProjectile_Validate(FVector StartLocation, FRotator TargetRotation)
{
	return true;
}


void AACMatchStickWeapon::SeverRPCCreateProjectile_Implementation(FVector StartLocation, FRotator TargetRotation)
{
	AC_LOG(LogACNetwork, Log, TEXT(""));
	CreateProjectile(StartLocation, TargetRotation);
}

void AACMatchStickWeapon::CreateProjectile(FVector StartLocation, FRotator TargetRotation)
{
	AC_LOG(LogACNetwork, Log, TEXT(""));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());

	if (UWorld* World = GetWorld()) 
	{
		AACProjectileFireBall* Projectile = GetWorld()->SpawnActor<AACProjectileFireBall>(FireBallProjectileClass, StartLocation, TargetRotation, SpawnParams);
		if (Projectile) 
		{
			Projectile->SetOwner(GetOwner());
			// --CurrentAmmoCount;
		}
	}
}

bool AACMatchStickWeapon::OnFire(bool bIsDead)
{
	bool Result = Super::OnFire(bIsDead);

	if (false == Result)
	{
		AC_LOG(LogACNetwork, Log, TEXT("1"));
		return false;
	}
	if( Character == nullptr) 
	{
		AC_LOG(LogACNetwork, Log, TEXT("2"));
		return false;
	}
	if (Character->GetController() == nullptr)
	{
		AC_LOG(LogACNetwork, Log, TEXT("3"));
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

		Result = GetWorld()->LineTraceSingleByChannel( TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		const FTransform StartTransform = GetMuzzleTransform();
		const FVector StartLocation = StartTransform.GetLocation();
		SeverRPCCreateProjectile(StartLocation, Character->GetCameraForwardDirection().Rotation());
		CreateProjectile(StartLocation, Character->GetCameraForwardDirection().Rotation());
		// if (false == Result)
		// {
		// 	SeverRPCCreateProjectile(StartLocation, Character->GetCameraForwardDirection().Rotation());
		// }
		// 
		// const FVector HitTarget = TraceHitResult.ImpactPoint;
		// const FVector TargetLocation = HitTarget - StartTransform.GetLocation();
		// const FRotator TargetRotation = TargetLocation.Rotation();
		// SeverRPCCreateProjectile(StartTransform.GetLocation(), TargetRotation);

		SetAmmo(CurrentAmmoCount - 1);
	}
	return Result;
}
