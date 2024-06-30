
#include "ACCharacterStatComponent.h"
#include "Manager/ACDataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ACCharacterBase.h"
#include "Data/ACCharacterRoleTypeData.h"
#include "Net/UnrealNetwork.h"
#include "ArcadeProtoType.h"

UACCharacterStatComponent::UACCharacterStatComponent()
{
	bWantsInitializeComponent = true;
}

void UACCharacterStatComponent::Reset()
{
	SetHP(CharacterStat.MaxHp);
}

void UACCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	const ECharacterType CharacterType = Cast<AACCharacterBase>(GetOwner())->GetCharacterType();

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		SetCharacterStat(DataManager->GetCharacterStat(CharacterType));
	}

	SetHP(CharacterStat.MaxHp);

	SetIsReplicated(true);
}

void UACCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UACCharacterStatComponent, CurrentHp);
	DOREPLIFETIME_CONDITION(UACCharacterStatComponent, CharacterStat, COND_OwnerOnly);
}

// Called when the game starts
void UACCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


float UACCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.0f, InDamage);
	SetHP(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		if (OnHpZero.IsBound()) 
		{
			OnHpZero.Broadcast();
		}
	}

	return ActualDamage;
}

void UACCharacterStatComponent::SetHP(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, CharacterStat.MaxHp);
	if (OnHpChanged.IsBound()) 
	{
		OnHpChanged.Broadcast(NewHp, CharacterStat.MaxHp);
	}
}

void UACCharacterStatComponent::OnRep_CurrentHp()
{
	if (OnHpChanged.IsBound()) 
	{
		OnHpChanged.Broadcast(CurrentHp, CharacterStat.MaxHp);
	}
	if (CurrentHp <= 0.0f)
	{
		OnHpZero.Broadcast();
	}
}

void UACCharacterStatComponent::OnRep_CharacterStat()
{
	if (OnStatChanged.IsBound())
	{
		OnStatChanged.Broadcast(CharacterStat);
	}
	
}
