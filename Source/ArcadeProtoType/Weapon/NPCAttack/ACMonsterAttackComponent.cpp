#include "Weapon/NPCAttack/ACMonsterAttackComponent.h"
#include "Engine/DamageEvents.h"
#include "Manager/ACDataManager.h"
#include "Character/ACCharacterPlayer.h"
#include "Character/ACCharacterNonPlayer.h"


UACMonsterAttackComponent::UACMonsterAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	AttackDamageAmount = 0.f;
	AttachPoint = FName("AttachPoint");
	AttackRange =0.f;
	// MonsterType = EMonsterType::eMinion;
}

void UACMonsterAttackComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		EMonsterType MonsterType = Cast<AACCharacterNonPlayer>(GetOwner())->GetMonsterType();
		if (DataManager->IsMonsterDataValid()) 
		{
			FACMonsterData MonsterData = DataManager->GetMonsterData(MonsterType);
			AttachPoint = MonsterData.AttachPoint;
			AttackDamageAmount = MonsterData.AttackDamage;
			AttackRange = MonsterData.AttackRange;
		}
	}
}

void UACMonsterAttackComponent::BeginAttack()
{

}

void UACMonsterAttackComponent::EndAttack()
{

}

void UACMonsterAttackComponent::OnAttack()
{

}

void UACMonsterAttackComponent::ApplyDamage(APawn* Target)
{
	if (AACCharacterPlayer* Player = Cast<AACCharacterPlayer>(Target))
	{
		if (Player->IsDead())
		{
			return;
		}
		FDamageEvent DamageEvent;
		Player->TakeDamage(AttackDamageAmount, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
	}
}

