#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ACDataUtility.h"
#include "ACCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FACCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FACCharacterStat() :CharacterType(ECharacterType::eMantus), MaxHp(0.0f), AttackDamage(0.0f), MaxDashDistance(0.0f), ShootRange(0.0f), MovementSpeed(0.0f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	ECharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxDashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float ShootRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;
	
	UPROPERTY(EditAnywhere, Category = WeaponClass)
	TSubclassOf<class AACWeaponBase> WeaponClass;
/*
	FACCharacterStatoperator+(const FACCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FABCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FACCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}*/
};