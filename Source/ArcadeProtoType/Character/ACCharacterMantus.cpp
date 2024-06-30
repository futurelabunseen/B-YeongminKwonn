
#include "Character/ACCharacterMantus.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Weapon/ACTreeSapWeapon.h"
#include "Components/ACShootingComponent.h"
#include "ArcadeProtoType.h"

AACCharacterMantus::AACCharacterMantus()
{
	// Material
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstanceRef1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Arcade/CharacterAssets/Materials/MI_CharacterMantus.MI_CharacterMantus'"));
	if (MaterialInstanceRef1.Object) 
	{
		BodyMaterialInstance = MaterialInstanceRef1.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialInstanceRef2(TEXT("/Script/Engine.Material'/Game/Arcade/CharacterAssets/Materials/M_AnimMan_Eyes.M_AnimMan_Eyes'"));
	if (MaterialInstanceRef2.Object)
	{
		GetMesh()->SetMaterial(10, MaterialInstanceRef2.Object);
	}
	
	CharacterType = ECharacterType::eMantus;
}

void AACCharacterMantus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AACCharacterMantus::Shoot);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AACCharacterMantus::EndShoot);
}

void AACCharacterMantus::Shoot()
{
	Super::Shoot();

	if (ECharacterControlType::Aim == CurrentCharacterControlType)
	{
		if (ShootingComponent)
		{
			ShootingComponent->OnFire();
		}
	}
}

void AACCharacterMantus::EndShoot()
{
	if (ECharacterControlType::Aim == CurrentCharacterControlType) 
	{
		if (ShootingComponent)
		{
			ShootingComponent->EndFire();
		}
	}
}

void AACCharacterMantus::CompletedAim()
{
	if (ECharacterControlType::Aim == CurrentCharacterControlType)
	{
		if (ShootingComponent)
		{
			ShootingComponent->EndFire();
		}
	}

	Super::CompletedAim();
}

