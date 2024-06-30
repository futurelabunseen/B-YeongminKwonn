#include "Character/ACCharacterCollie.h"
#include "EnhancedInputComponent.h"
#include "Weapon/ACMatchStickWeapon.h"
#include "Components/ACShootingComponent.h"
#include "ArcadeProtoType.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AACCharacterCollie::AACCharacterCollie()
{
	// Material
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstanceRef1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Arcade/CharacterAssets/Materials/MI_CharacterCollie.MI_CharacterCollie'"));
	if (MaterialInstanceRef1.Object)
	{
		BodyMaterialInstance = MaterialInstanceRef1.Object;
	}

	GetMesh()->SetMaterial(10, EyeMaterialInstance);

	// DeadEffect Material
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstanceRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Arcade/Material/MI_DeadCharacterEffect_Collie.MI_DeadCharacterEffect_Collie'"));
	if (MaterialInstanceRef.Succeeded())
	{
		DeadMaterialInstance = MaterialInstanceRef.Object;

		bIsDead = false;
	}
	CharacterType = ECharacterType::eCollie;
}

void AACCharacterCollie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started , this, &AACCharacterCollie::Shoot);
}

void AACCharacterCollie::Shoot()
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
