
#include "Character/ACCharacterNonPlayer.h"
#include "HUD/ACCharacterHUD.h"
#include"Stat/ACCharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ACCharacterPlayer.h"
#include "Player/ACPlayerController.h"
#include "ArcadeProtoType.h"
#include "EngineUtils.h"
#include "Game/ACGameState.h"

AACCharacterNonPlayer::AACCharacterNonPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Ancient_Golem/Mesh/SK_Ancient_Golem.SK_Ancient_Golem'"));
	if (CharacterBodyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterBodyMeshRef.Object);
	}

	// AnimInstance
	// static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Arcade/Blueprint/ABP_Monster.ABP_Monster_C'"));
	// if (AnimInstanceClassRef.Class)
	// {
	// 	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	// }

	bIsTargetChanged = false;
	Target = nullptr;

	MonsterType = EMonsterType::eMinion;
}


void AACCharacterNonPlayer::SetupHUDWidget(AACCharacterHUD* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateHpWidget(Stat->GetCurrentHp(), Stat->GetCharacterStat().MaxHp);
	
	}
}

TArray<AActor*> AACCharacterNonPlayer::GetStuckProjectile() const
{
	return StuckProjectileArray;
}

void AACCharacterNonPlayer::AddStuckProjectileArray(AActor* StuckProjectile)
{
	StuckProjectileArray.Add(StuckProjectile);
}

void AACCharacterNonPlayer::SetDead()
{
	if (HasAuthority()) 
	{
		for (AActor* Actor : StuckProjectileArray)
		{
			if (Actor)
			{
				Actor->Destroy();
			}
		}
	}

	Super::SetDead();
}

void AACCharacterNonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void AACCharacterNonPlayer::ChangeTarget()
{
	if (nullptr == GetWorld())
	{
		return;
	}

	if (AACGameState* GameState = Cast<AACGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		const int PlayerCount = GameState->GetTargetArray().Num();

		if (PlayerCount < 2) 
		{
			return;
		}

		// 처음 공격한 플레이어의 인덱스를 피해서 선택
		int NewIndex = (LastTargetIndex == 0) ? 1 : 0;
		if (AACCharacterPlayer* PotentialTarget = Cast<AACCharacterPlayer>(GameState->GetTargetArray()[NewIndex]->GetPawn())) 
		{
			// 선택한 플레이어가 죽은 상태라면 다른 플레이어를 선택
			if (PotentialTarget->IsDead())
			{
				NewIndex = (NewIndex == 0) ? 1 : 0;
				PotentialTarget = Cast<AACCharacterPlayer>(GameState->GetTargetArray()[NewIndex]->GetPawn());
			}

			if (PotentialTarget && false == PotentialTarget->IsDead())
			{
				LastTargetIndex = NewIndex;
				AttackTarget = Cast<APawn>(PotentialTarget);
			}
		}
	}
}

void AACCharacterNonPlayer::SetTarget(AACCharacterPlayer* Player)
{
	Target = Player;
}

APawn* AACCharacterNonPlayer::GetTarget() const
{
	return Target;
}

APawn* AACCharacterNonPlayer::GetAttackTarget()
{
	if (AACCharacterPlayer * NewTarget = Cast<AACCharacterPlayer>(AttackTarget))
	{
		if (NewTarget->IsDead()) 
		{
			ChangeTarget();
			return AttackTarget;
		}
	}

	return AttackTarget;
}

float AACCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

// 로밍 상태일 때 도착했다고 체크하는 허용 범위
float AACCharacterNonPlayer::GetDestinationTolerance_InRoaming()
{
	return 200.f;
}

// 타겟을 감지할 수 있는 거리 -> Sight
float AACCharacterNonPlayer::GetDetectRadius()
{
	return 500.f;
}

// 패트롤 상태일 때, 더 이상 타겟에게 가까이 오지 않는다고 설정한 범위
float AACCharacterNonPlayer::GetDestinationTolerance_InPatrol()
{
	return 500.f;
}

// AttackRange
float AACCharacterNonPlayer::GetAIAttackRange()
{
	return 1000.f;
}

void AACCharacterNonPlayer::SetAIAttackDelegate(const FNPCAttackFinished& InOnAttackFinished)
{
}

void AACCharacterNonPlayer::AttackByAI() 
{

}