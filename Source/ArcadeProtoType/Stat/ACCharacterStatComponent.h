#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ACCharacterStat.h"
#include "ACCharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /*CurrentHp*/, float /*MaxHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChangedDelegate, const FACCharacterStat& /*BaseStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADEPROTOTYPE_API UACCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACCharacterStatComponent();
	void Reset();

protected:
	virtual void BeginPlay() override;
	void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	FORCEINLINE const FACCharacterStat& GetCharacterStat() const { return CharacterStat; }
	FORCEINLINE void SetCharacterStat(FACCharacterStat InCharacterStat) { CharacterStat = InCharacterStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	float ApplyDamage(float InDamage);

protected:
	void SetHP(float NewHp);

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHp, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UFUNCTION()
	void OnRep_CurrentHp();

	UPROPERTY(Transient, ReplicatedUsing = OnRep_CharacterStat,VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FACCharacterStat CharacterStat;

	UFUNCTION()
	void OnRep_CharacterStat();
};
