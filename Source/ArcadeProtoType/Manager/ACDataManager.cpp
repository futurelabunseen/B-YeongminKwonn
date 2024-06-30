
#include "Manager/ACDataManager.h"

UACDataManager::UACDataManager()
{
	// Projectile Type
	static ConstructorHelpers::FObjectFinder<UDataTable> ProjectileDataRef(TEXT("/Game/Arcade/Data/DataTable/DT_ACProjectileData.DT_ACProjectileData"));
	TArray<FACProjectileData> ProjectileDataTable;
	if (nullptr != ProjectileDataRef.Object)
	{
		const UDataTable* DataTable = ProjectileDataRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, ProjectileDataTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FACProjectileData*>(Value);
			}
		);

		Algo::Transform(ProjectileDataTable, ProjectileDataMap,
			[](const FACProjectileData& Val)
			{
				return TPair<EProjectileType, FACProjectileData>(Val.ProjectileType, Val);
			}
		);
	}


	// WeaponType
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDataRef(TEXT("/Game/Arcade/Data/DataTable/DT_ACWeaponData.DT_ACWeaponData"));
	TArray<FACWeaponData> WeaponDataTable2;
	if (nullptr != WeaponDataRef.Object)
	{
		const UDataTable* DataTable = WeaponDataRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, WeaponDataTable2,
			[](uint8* Value)
			{
				return *reinterpret_cast<FACWeaponData*>(Value);
			}
		);

		// Algo::Transform(WeaponDataTable2, WeaponDataMap,
		// 	[](const FACWeaponData& Val)
		// 	{
		// 		return TPair<EWeaponType, FACWeaponData>(Val.WeaponType, Val);
		// 	}
		// );
	}
	WeaponDataMap.Add(EWeaponType::eTreeSapWeapon, WeaponDataTable2[0]);
	WeaponDataMap.Add(EWeaponType::eMatchStickWeapon, WeaponDataTable2[1]);

	// CharacterType
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStatDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Arcade/Data/DataTable/DT_CharacterStatData.DT_CharacterStatData'"));
	TArray< FACCharacterStat> CharacterStatTable;
	if (nullptr != CharacterStatDataTableRef.Object)
	{
		const UDataTable* DataTable = CharacterStatDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FACCharacterStat*>(Value);
			}
		);

		Algo::Transform(CharacterStatTable, CharacterStatDataMap,
			[](const FACCharacterStat& Val)
			{
				return TPair<ECharacterType, FACCharacterStat>(Val.CharacterType, Val);
			}
		);
	}

	// MonsterType
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Arcade/Data/DataTable/DT_ACMonsterData.DT_ACMonsterData'"));
	TArray< FACMonsterData> MonsterTable;
	if (nullptr != MonsterDataTableRef.Object)
	{
		const UDataTable* DataTable = MonsterDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, MonsterTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FACMonsterData*>(Value);
			}
		);

		Algo::Transform(MonsterTable, MonsterDataMap,
			[](const FACMonsterData& Val)
			{
				return TPair<EMonsterType, FACMonsterData>(Val.MonsterType, Val);
			}
		);
	}

	// BossAttackDataArray
	static ConstructorHelpers::FObjectFinder<UDataTable> BossAttackDataTableRef(TEXT("/Game/Arcade/Data/DataTable/DT_ACBossAttack.DT_ACBossAttack"));
	if (BossAttackDataTableRef.Succeeded())
	{
		BossAttackData = BossAttackDataTableRef.Object;
	}

	// MinionData
	static ConstructorHelpers::FObjectFinder<UDataTable> MinionSpawnDataTableRef(TEXT("/Game/Arcade/Data/DataTable/DT_MinionSpawnData.DT_MinionSpawnData"));
	if (MinionSpawnDataTableRef.Succeeded())
	{
		MinionSpawnData = MinionSpawnDataTableRef.Object;
	}

	// TimerData
	static ConstructorHelpers::FObjectFinder<UDataTable> TimerDataTableRef(TEXT("/Game/Arcade/Data/DataTable/DT_TimerData.DT_TimerData"));
	if (TimerDataTableRef.Succeeded())
	{
		TimerData = TimerDataTableRef.Object;
	}

	// CharacterRespawn
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterRespawnDataTableRef(TEXT("/Game/Arcade/Data/DataTable/DT_CharacterRespawnData.DT_CharacterRespawnData"));
	if (CharacterRespawnDataTableRef.Succeeded())
	{
		CharacterRespawnData = CharacterRespawnDataTableRef.Object;
	}

	// CharacterProfileImageData
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterImageDataTableRef(TEXT("/Game/Arcade/Data/DataTable/DT_CharacterProfileImage.DT_CharacterProfileImage"));
	if (CharacterImageDataTableRef.Succeeded())
	{
		CharacterProfileImageData = CharacterImageDataTableRef.Object;
	}
	// GameRuleData
	// static ConstructorHelpers::FObjectFinder<UDataTable> GameRuleDataTableRef(TEXT("/Game/Arcade/Data/DataTable/DT_ACBossAttack.DT_ACBossAttack"));
	// if (GameRuleDataTableRef.Succeeded())
	// {
	// 	GameRuleData = GameRuleDataTableRef.Object;
	// }
}

void UACDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (BossAttackData)
	{
		static const FString ContextString(TEXT("BossAttackDataTable Context"));
		TArray<FACBossAttack*> RowArray;
		BossAttackData->GetAllRows(ContextString, RowArray);
		
		for (FACBossAttack* Row : RowArray)
		{
			if (Row)
			{
				BossAttackDataArray.Add(*Row);
			}
		}
	}


	if (MinionSpawnData)
	{
		static const FString ContextString(TEXT("MinionSpawnDataTable Context"));
		TArray<FACMinionSpawnData*> RowArray;
		MinionSpawnData->GetAllRows(ContextString, RowArray);

		for (FACMinionSpawnData* Row : RowArray)
		{
			if (Row)
			{
				MinionSpawnDataArray.Add(*Row);
			}
		}
	}

	if (TimerData)
	{
		static const FString ContextString(TEXT("TimerDataTable Context"));
		TArray<FACTimerData*> RowArray;
		TimerData->GetAllRows(ContextString, RowArray);

		for (FACTimerData* Row : RowArray)
		{
			if (Row)
			{
				TimerDataArray.Add(*Row);
			}
		}
	}

	if (CharacterRespawnData)
	{
		static const FString ContextString(TEXT("CharacterRespawnDataTable Context"));
		TArray<FACCharacterRespawnData*> RowArray;
		CharacterRespawnData->GetAllRows(ContextString, RowArray);

		for (FACCharacterRespawnData* Row : RowArray)
		{
			if (Row)
			{
				CharacterRespawnDataArray.Add(*Row);
			}
		}
	}

	if (CharacterProfileImageData)
	{
		static const FString ContextString(TEXT("CharacterProfileImageDataTable Context"));
		TArray<FACCharacterImage*> RowArray;
		CharacterProfileImageData->GetAllRows(ContextString, RowArray);

		for (FACCharacterImage* Row : RowArray)
		{
			if (Row)
			{
				CharacterProfileImageArray.Add(*Row);
			}
		}
	}
}

// void UACDataManager::Initialize(FSubsystemCollectionBase& Collection)
// {
// 	Super::Initialize(Collection);
// 
// 	// Projectile Data Map
// 	// for (const FName RowName : ProjectileDataTable->GetRowNames())
// 	// {
// 	// 	FACProjectileData* ProjectileData = ProjectileDataTable->FindRow<FACProjectileData>(RowName, RowName.ToString());
// 	// 	ProjectileDataMap.Add(ProjectileData->ProjectileType, ProjectileData);
// 	// }
// }

// UACCharacterRoleTypeData* UACDataManager::GetCharacterTypeData(ECharacterType CharacterType) const
// {
// 	return CharacterTypeDataMap[CharacterType];
// }

// FACProjectileData* UACDataManager::GetProjectileData(EProjectileType InProjectileType) const
// {
// 	return ProjectileDataMap[InProjectileType];
// }


