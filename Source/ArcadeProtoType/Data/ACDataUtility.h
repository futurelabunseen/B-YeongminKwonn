
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Game/GamePhase/ACGamePhase.h"
#include "ACDataUtility.generated.h"

// #define __DEBUG_DRAW

// UENUM()
// enum class ECharacterState : uint8
// {
// 	Alive,
// 	Respawning,
// 	Dead
// };

UENUM()
enum class ECharacterControlType : uint8
{
	Default,
	Aim
};

UENUM()
enum class ECharacterType : uint8
{
	eMantus,
	eCollie,
	eMonster
};

UENUM()
enum class EMonsterType : uint8
{
	eMinion,
	eBoss,
};

UENUM()
enum class EWeaponType : uint8
{
	eTreeSapWeapon,
	eMatchStickWeapon
};

UENUM()
enum class EProjectileType : uint8
{
	eLiquidBall,
	eFireBall,
	eCoopAttack
};



USTRUCT(BlueprintType)
struct FACTimerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FACTimerData() : TotalTime(0.f), MatchTime(0.f), TimerCheckRate(3.f)
	{  }

	FACTimerData(float _TotalTime, float _MatchTime) : TotalTime(_TotalTime), MatchTime(_MatchTime)
	{  }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MatchTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimerCheckRate;
};


USTRUCT(BlueprintType)
struct FACCharacterRespawnData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FACCharacterRespawnData() : MaxRespawnTime(0.f) , RespawnTimerRate(0.f)
	{  }
	FACCharacterRespawnData(float _MaxRespawnTime, float _RespawnTimerRate) : MaxRespawnTime(_MaxRespawnTime), RespawnTimerRate(_RespawnTimerRate)
	{  }
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRespawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RespawnTimerRate;
};

USTRUCT(BlueprintType)
struct FACCharacterImage : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FACCharacterImage() : CharacterType(ECharacterType::eMantus), CharacterProfileTexture(nullptr)
	{  }
	FACCharacterImage(UTexture _CharacterProfileTexture, ECharacterType _CharacterType) : CharacterProfileTexture(_CharacterProfileTexture), CharacterType(_CharacterType)
	{  }
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture> CharacterProfileTexture;
};


USTRUCT()
struct FACMinionSpawnData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	FACMinionSpawnData() : GamePhase(EACGamePhaseList::MinionCombat), SpawnRate(0.f), MaxSpawnCount(0){ }
	FACMinionSpawnData(EACGamePhaseList _GamePhase, float _SpawnRate, int _SpawnCount) : GamePhase(_GamePhase), SpawnRate(_SpawnRate), MaxSpawnCount(_SpawnCount)
	{  }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePhase")
	EACGamePhaseList GamePhase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MaxSpawnCount;

};


USTRUCT(BlueprintType)
struct FACBossAttack : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FACBossAttack() :LaserStartOffset(FVector(0.f, 0.f, 0.f)), LaserEndOffset(0.f), LaserWidth(0.f), 
		LaserHitWidth(0.f), LaserAttackDelayRate(0.1f), MaxLaserAttackCount(1), AttackLoopRate(0.f){  }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LaserStartOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaserEndOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaserWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaserHitWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaserAttackDelayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxLaserAttackCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackLoopRate;
};

USTRUCT(BlueprintType)
struct FACWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FACWeaponData() :WeaponType(EWeaponType::eTreeSapWeapon), WeaponMesh(nullptr), MaxAmmoCount(100), MaxShootRange(1000.f), ShootInterval(0.1f), ProjectileClass(nullptr) {  }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<class USkeletalMesh> WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxAmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float MaxShootRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ShootInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AACProjectileBase> ProjectileClass;
};

USTRUCT(BlueprintType)
struct FACProjectileData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FACProjectileData() 
	{
		ProjectileType = EProjectileType::eLiquidBall;
		ProjectileMesh = nullptr;
		NiagaraSystem = nullptr;
		ProjectileMaterial = nullptr;
		InitialSpeed = 3000.f;
		MaxSpeed = 3000.f;
		Bounciness = 3000.f;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EProjectileType ProjectileType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMesh> ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMaterialInstance> ProjectileMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Bounciness;
};

USTRUCT(BlueprintType)
struct FACMonsterData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FACMonsterData() : MonsterType(EMonsterType::eMinion), AttachPoint(FName("Effect")), AttackRange(0.f), AttackDamage(0.f) {  }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	EMonsterType MonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttachPoint")
	FName AttachPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackDamage;
};

UCLASS()
class ARCADEPROTOTYPE_API UACDataUtility : public UObject
{
	GENERATED_BODY()
public:
	UACDataUtility() {  }
	template <typename EnumType>
	static FORCEINLINE FString GetEnumName(const EnumType InValue)
	{
		return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(InValue));
	}

	template <typename EnumType>
	static FORCEINLINE FString GetEnumDisplayName(const EnumType InValue)
	{
		return StaticEnum<EnumType>()->GetDisplayNameTextByValue(static_cast<int64>(InValue)).ToString();
	}

};
