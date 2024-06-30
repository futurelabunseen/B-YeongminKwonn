// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ACCharacterRoleTypeData.generated.h"

UCLASS()
class ARCADEPROTOTYPE_API UACCharacterRoleTypeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UACCharacterRoleTypeData();

	// CharacterAttribute
	UPROPERTY(EditAnywhere, Category = Health)
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = Dash)
	float MaxDashDistance;

	UPROPERTY(EditAnywhere, Category = GravityDistance)
	float GravityScale;

	UPROPERTY(EditAnywhere, Category = CameraFOV)
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = CameraFOV)
	float AimFOV;

	UPROPERTY(EditAnywhere, Category = CameraFOV)
	float ZoomInterpSpeed;
	
	// Weapon
	UPROPERTY(EditAnywhere, Category = WeaponClass)
	TSubclassOf<class AACWeaponBase> WeaponClass;
};
