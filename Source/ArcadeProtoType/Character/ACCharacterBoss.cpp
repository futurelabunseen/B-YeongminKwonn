#include "Character/ACCharacterBoss.h"
#include "Animation/ACBossAnimInstance.h"
#include "ArcadeProtoType.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/NPCAttack/ACBossAttackLaserComponent.h"
#include "ArcadeProtoType.h"
#include "Manager/ACSpawnManager.h"
#include "Manager/ACMonsterManager.h"
#include "Game/ACGameState.h"
#include "Stat/ACCharacterStatComponent.h"
#include "HUD/ACCharacterHUD.h"
#include "Player/ACPlayerController.h"
#include "kismet/GameplayStatics.h"

AACCharacterBoss::AACCharacterBoss()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Ancient_Golem/Mesh/SK_Ancient_Golem.SK_Ancient_Golem'"));
	if (CharacterBodyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterBodyMeshRef.Object);
	}

	HeadComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterHeadMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Ancient_Golem/Mesh/SK_Ancient_Golem_Head_Wheel.SK_Ancient_Golem_Head_Wheel'"));
	if (CharacterHeadMeshRef.Object)
	{
		HeadComponent->SetSkeletalMesh(CharacterHeadMeshRef.Object);
	}

	WheelComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WheelComponent"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterWheelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Ancient_Golem/Mesh/SK_Ancient_Golem_Moving_Wheels.SK_Ancient_Golem_Moving_Wheels'"));
	if (CharacterWheelMeshRef.Object)
	{
		WheelComponent->SetSkeletalMesh(CharacterWheelMeshRef.Object);
	}

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Arcade/Blueprint/ABP_ACBoss.ABP_ACBoss_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	bisFiring = false;

	AttackLaserComponent = CreateDefaultSubobject<UACBossAttackLaserComponent>(TEXT("AttackLaser"));

	MonsterType = EMonsterType::eBoss;

	// DataTable
	RotationRate = 20.0f;
	AttackIterval = 0.04f;
	RotateIterval = 0.04f;
}

void AACCharacterBoss::SetupHUDWidget(AACCharacterHUD* InHUDWidget)
{
	Super::SetupHUDWidget(InHUDWidget);

	if (InHUDWidget)
	{
		Stat->OnHpChanged.AddUObject(InHUDWidget, &AACCharacterHUD::UpdateMonsterHpBar);
	}
}

void AACCharacterBoss::BeginPlay()
{
	Super::BeginPlay();

	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		true);

	const FName HeadSocketName = FName("headSocket");
	const FName WheelSocketName = FName("AttachPoint");
	
	HeadComponent->AttachToComponent(GetMesh(), AttachmentRules, HeadSocketName);
	WheelComponent->AttachToComponent(GetMesh(), AttachmentRules, WheelSocketName);

	if (UACBossAnimInstance* AnimInstance = Cast<UACBossAnimInstance>(GetMesh()->GetAnimInstance())) 
	{
		AnimInstance->OnEndLaserAttackAnimationEvent.AddUObject(this, &AACCharacterBoss::LaserAttack);
	}

	FTimerHandle timerHandle;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (AACPlayerController* ArcadePlayerController = Cast<AACPlayerController>(Iterator->Get()))
		{
			if (AACCharacterHUD* HUD = Cast<AACCharacterHUD>(ArcadePlayerController->GetHUD()))
			{
				SetupHUDWidget(HUD);
			}
		}
	}

	FTimerHandle AnnimationtimerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
		{
			PlayLaserAttackAnimation();
		}
	), 3.f, false);
}

void AACCharacterBoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACCharacterBoss, ServerRotationYaw); 
	DOREPLIFETIME(AACCharacterBoss, bStopRotation);
}

void AACCharacterBoss::Tick(float DeltaTimes)
{
	Super::Tick(DeltaTimes);
	
}

void AACCharacterBoss::LaserAttack()
{
	// Rotate
	GetWorld()->GetTimerManager().SetTimer(LaserAttackTimerHandle, this, &AACCharacterBoss::UpdateLaserAttackRotate, AttackIterval, true);
	
	//Attack
	AttackLaserComponent->BeginAttack();

	// SetTarget
	ChangeTarget();
}


void AACCharacterBoss::UpdateLaserAttackRotate()
{
	if (bStopRotation == true) 
	{
		return;
	}
	// 서버에서 로직 처리
	if (HasAuthority())
	{
		float RotationAmount = RotationRate * RotateIterval;
		AddActorLocalRotation(FRotator(0.0f, RotationAmount, 0.0f));
		// 현재 보스의 회전 각도를 서버 변수에 저장
		ServerRotationYaw = RootComponent->GetComponentRotation().Yaw;

		// 총 회전량 업데이트
		TotalRotation += RotationAmount;
		// 보스가 360도 회전했는지 확인
		if (TotalRotation >= 360.0f)
		{
			// 총 회전량 초기화
			TotalRotation = 0.0f;
		}
	}
	else
	{
		// 클라이언트에서 시간 업데이트
		ClientTimeSinceUpdate += RotateIterval;
		// 마지막 업데이트와의 시간 차가 너무 작으면 리턴
		if (ClientTimeBetweenLastUpdate < KINDA_SMALL_NUMBER)
		{
			return;
		}

		// 서버 회전각과 회전 속도를 기반으로 예상 회전 각도 계산
		const float EstimateRotationYaw = ServerRotationYaw + RotationRate * ClientTimeBetweenLastUpdate;
		// 보간 비율 계산
		const float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdate;
		// 현재 클라이언트의 회전 각도 가져오기
		FRotator ClientRotator = RootComponent->GetComponentRotation();
		// 서버의 예상 회전 각도와 현재 클라이언트의 회전 각도를 보간
		const float ClientNewYaw = FMath::Lerp(ServerRotationYaw, EstimateRotationYaw, LerpRatio);
		// 보간된 회전 각도로 클라이언트의 회전 업데이트
		ClientRotator.Yaw = ClientNewYaw;
		RootComponent->SetWorldRotation(ClientRotator);
	}
}


void AACCharacterBoss::OnRep_ServerRotationYaw()
{
	FRotator NewRotator = RootComponent->GetComponentRotation();
	NewRotator.Yaw = ServerRotationYaw;
	RootComponent->SetWorldRotation(NewRotator);

	ClientTimeBetweenLastUpdate = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0.0f;
}

void AACCharacterBoss::PlayLaserAttackAnimation()
{
	if (UACBossAnimInstance* AnimInstance = Cast<UACBossAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->PlayLaserAttackMontage();
	}
}

void AACCharacterBoss::SetDead()
{
	Super::SetDead();

	if (UACMonsterManager* MonsterManager = GetWorld()->GetSubsystem<UACMonsterManager>())
	{
		MonsterManager->DestroyBossMonster();
	}

	if (AACGameState* ArcadeGameState = Cast<AACGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		ArcadeGameState->SetGamePhase(EACGamePhaseList::GameOver);
	}
}

void AACCharacterBoss::SetStopRotation(bool InbStopRotation)
{
	bStopRotation = InbStopRotation;
}
