#include "ACShootingComponent.h"
#include "Data/ACDataUtility.h"
#include "Manager/ACDataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Data/ACCharacterRoleTypeData.h"
#include "Character/ACCharacterPlayer.h"
#include "Weapon/ACWeaponBase.h"
#include "Weapon/ACTreeSapWeapon.h"
#include "Weapon/ACMatchStickWeapon.h"
#include "Player/ACPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "ArcadeProtoType.h"
#include "Data/ACCharacterStat.h"
#include "HUD/ACCharacterHUD.h"

UACShootingComponent::UACShootingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	CurrentFOV = DefaultFOV;
}

void UACShootingComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Character = Cast<AACCharacterPlayer>(GetOwner());

	if (nullptr == Character)
	{
		return;
	}

	SetIsReplicated(true);
}

void UACShootingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UACShootingComponent::SpawnWeapon()
{
	if (Character == nullptr)
		return;

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		FACCharacterStat CharacterTypeData = DataManager->GetCharacterStat(Character->GetCharacterType());

		if (Character->HasAuthority() && GetOwner() && Weapon == nullptr)
		{
			if (GetWorld() && GetOwner()->HasAuthority())
			{
				Weapon = GetWorld()->SpawnActor<AACWeaponBase>(CharacterTypeData.WeaponClass);
				ensure(Weapon);
				Weapon->SetOwner(GetOwner());
				Weapon->Equip();
			}
		}
	}
}

int UACShootingComponent::GetCurrentAmmo() const
{
	if (Weapon)
	{
		return Weapon->GetCurrentAmmo();
	}
	else
	{
		return 0;
	}
}

int UACShootingComponent::GetMaxAmmo() const
{
	if (Weapon)
	{
		return Weapon->GetMaxAmmo();
	}
	else
	{
		return 0;
	}
}

void UACShootingComponent::ResetWeapon()
{
	if (Weapon)
	{
		Weapon->ResetWeapon();
	}
}

void UACShootingComponent::SetWeaponHiddenInGame(bool bIsHidden)
{
	if (Weapon)
	{
		Weapon->SetActorHiddenInGame(bIsHidden);
	}
}


void UACShootingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UACShootingComponent, Weapon);
}


void UACShootingComponent::SetZoomFOV(float DeltaTime)
{
	if (Character && Character->GetController())
	{
		if (Character->IsLocallyControlled())
		{
			CurrentFOV = Character->IsAiming() ?
				FMath::FInterpTo(CurrentFOV, AimFOV, DeltaTime, ZoomInterpSpeed)
				: FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);

			Cast<AACPlayerController>(Character->GetController())->PlayerCameraManager->SetFOV(CurrentFOV);
		}
	}
}


void UACShootingComponent::OnFire()
{
	if (nullptr == Weapon) 
	{
		AC_SUBLOG(LogACNetwork, Log, TEXT("1"));
		return;
	}
	if(nullptr == GetWorld()  || nullptr == Character->GetController())
	{
		AC_SUBLOG(LogACNetwork, Log, TEXT("2"));
		return;
	}

	if( nullptr == Character) 
	{
		AC_SUBLOG(LogACNetwork, Log, TEXT("3"));
		return;
	}

	const bool IsDead = Character->IsDead();

	switch (Weapon->GetWeaponType())
	{
	case EWeaponType::eMatchStickWeapon:
		Cast<AACMatchStickWeapon>(Weapon)->OnFire();
		break;
	case EWeaponType::eTreeSapWeapon:
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimerHandle, FTimerDelegate::CreateLambda([this, IsDead]()
			{
				Weapon->OnFire(IsDead);
			}
		), Cast<AACTreeSapWeapon>(Weapon)->GetShootInterval(), true, 0.f);
		break;
	default:
		break;
	}
}

void UACShootingComponent::EndFire()
{
	if (nullptr == GetWorld() || nullptr == Character || nullptr == Character->GetController())
	{
		return;
	}

	if (Weapon)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoFireTimerHandle);
	}
}


void UACShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetZoomFOV(DeltaTime);
	OnDetecting();
}

void UACShootingComponent::OnDetecting()
{
	if (nullptr == Weapon || nullptr == Character)
		return;

	if (Weapon->GetWeaponType() == EWeaponType::eMatchStickWeapon && Character->IsAiming())
	{
		if (Cast<AACMatchStickWeapon>(Weapon)->IsDetecting())
		{
			OnAimStatusChanged.Broadcast(EAimType::DETECT);
		}
		else
		{
			OnAimStatusChanged.Broadcast(EAimType::AIM);
		}
	}
}

void UACShootingComponent::OnRep_Weapon()
{
	AC_SUBLOG(LogACNetwork, Log, TEXT(""));
}
