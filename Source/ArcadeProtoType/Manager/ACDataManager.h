// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ACDataUtility.h"
#include "Data/ACCharacterStat.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ACDataManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UACDataManager();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	FORCEINLINE FACCharacterStat GetCharacterStat(ECharacterType InCharacterType) const { return CharacterStatDataMap[InCharacterType]; }
	FORCEINLINE FACWeaponData GetWeaponData(EWeaponType InWeaponType) const { return WeaponDataMap[InWeaponType]; }
	FORCEINLINE FACProjectileData GetProjectileData(EProjectileType InProjectileType) const { return ProjectileDataMap[InProjectileType]; }
	FORCEINLINE FACMonsterData GetMonsterData(EMonsterType InMonsterType) const { return MonsterDataMap[InMonsterType]; }
	FORCEINLINE FACBossAttack GetBossAttackData(int32 Index) const { return BossAttackDataArray[Index]; }
	FORCEINLINE TArray<FACMinionSpawnData> GetMinionSpawnDataArray() const { return MinionSpawnDataArray; }
	FORCEINLINE FACCharacterRespawnData GetCharacterRespawnData(int32 Index) const { return CharacterRespawnDataArray[Index]; }
	FORCEINLINE FACTimerData GetTimerData(int32 Index) const { return TimerDataArray[Index]; }
	FORCEINLINE FACCharacterImage GetCharacterImageData(int32 Index) const { return CharacterProfileImageArray[Index]; }
	
	FORCEINLINE bool  IsCharacterStatValid() const { return CharacterStatDataMap.Num() == 0 ? false : true; }
	FORCEINLINE bool  IsWeaponDataValid() const { return WeaponDataMap.Num() == 0 ? false : true; }
	FORCEINLINE bool  IsProjectileDataValid() const { return ProjectileDataMap.Num() == 0 ? false : true; }
	FORCEINLINE bool  IsMonsterDataValid() const { return MonsterDataMap.Num() == 0 ? false : true; }
	FORCEINLINE bool  IsBossAttackDataValid() const { return BossAttackDataArray.Num() == 0 ? false : true; }
	FORCEINLINE bool  IsMinionSpawnDataValid() const { return MinionSpawnDataArray.Num() == 0 ? false : true; }
	FORCEINLINE bool  IsCharacterRespawnDataValid() const { return CharacterRespawnDataArray.Num() == 0 ? false : true; }
	FORCEINLINE bool  IsTimerDataValid() const { return TimerDataArray.Num() == 0 ? false : true; }
	FORCEINLINE bool  IsCharacterImageValid() const { return CharacterProfileImageArray.Num() == 0 ? false : true; }


private:
	UPROPERTY()
	TMap<ECharacterType, FACCharacterStat> CharacterStatDataMap;
	UPROPERTY()
	TMap<EWeaponType, FACWeaponData> WeaponDataMap;
	UPROPERTY()
	TMap<EProjectileType, FACProjectileData> ProjectileDataMap;
	UPROPERTY()
	TMap<EMonsterType, FACMonsterData> MonsterDataMap;


	TObjectPtr<UDataTable> BossAttackData;
	UPROPERTY()
	TArray<FACBossAttack> BossAttackDataArray;


	TObjectPtr<UDataTable> MinionSpawnData;
	UPROPERTY()
	TArray<FACMinionSpawnData> MinionSpawnDataArray;


	TObjectPtr<UDataTable> CharacterRespawnData;
	UPROPERTY()
	TArray<FACCharacterRespawnData> CharacterRespawnDataArray;

	TObjectPtr<UDataTable> TimerData;
	UPROPERTY()
	TArray<FACTimerData> TimerDataArray;

	TObjectPtr<UDataTable> CharacterProfileImageData;
	UPROPERTY()
	TArray<FACCharacterImage> CharacterProfileImageArray;
};
