#include "Weapon/NPCAttack/ACBossAttackLaserComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/ACCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ACCharacterBoss.h"
#include "GameFramework/Character.h"
#include "Engine/DamageEvents.h"
#include "ArcadeProtoType.h"
#include "Net/UnrealNetwork.h"
#include "Manager/ACDataManager.h"

UACBossAttackLaserComponent::UACBossAttackLaserComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	// LaserParticleEffect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> LaserParticleEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Arcade/Weapons/NPCAttack/P_Laser.P_Laser'"));
	if (LaserParticleEffectRef.Object)
	{
		LaserParticleEffect = LaserParticleEffectRef.Object;
	}

	// LaserParticleEffectComponent
	LaserParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticleComponent"));

	// LaserHitParticleEffect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> LaserHitParticleEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Arcade/Weapons/NPCAttack/P_Laser_Hit.P_Laser_Hit'"));
	if (LaserHitParticleEffectRef.Object)
	{
		LaserHitParticleEffect = LaserHitParticleEffectRef.Object;
	}

	// LaserParticleEffectComponent
	LaserHitParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserHitParticleComponent"));

	LaserStartOffset = FVector(100.0f, 0.0f, 0.0f);
	LaserEndOffset = 0.9f;
	LaserWidth = 0.8f;
	LaserHitWidth = 1.0f;

	bLaserSpawned = false;
	bLaserHitSpawned = false;

	// MonsterType = EMonsterType::eBoss;
}

void UACBossAttackLaserComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem<UACDataManager>(GetWorld()->GetGameInstance()))
	{
		if (true == DataManager->IsBossAttackDataValid()) 
		{
			const int Index = 0;
			FACBossAttack BossAttack = DataManager->GetBossAttackData(Index);
			LaserStartOffset = BossAttack.LaserStartOffset;
			LaserEndOffset = BossAttack.LaserEndOffset;
			LaserWidth = BossAttack.LaserWidth;
			LaserHitWidth = BossAttack.LaserHitWidth;
			LaserAttackDelayRate = BossAttack.LaserAttackDelayRate;
			MaxLaserAttackCount = BossAttack.MaxLaserAttackCount;
			AttackLoopRate = BossAttack.AttackLoopRate;
		}
	}
	
}

void UACBossAttackLaserComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetIsReplicated(true);
	
}

void UACBossAttackLaserComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UACBossAttackLaserComponent, bIsActiveLaser);
	DOREPLIFETIME(UACBossAttackLaserComponent, EndLocation);
}



void UACBossAttackLaserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}


void UACBossAttackLaserComponent::BeginAttack()
{
	SetActiveLaser(true);
	if (GetOwner()->HasAuthority())
	{
		SetStartLocation();
	}
	
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &UACBossAttackLaserComponent::OnAttack, AttackLoopRate, true, AttackLoopRate);
}

void UACBossAttackLaserComponent::SpawnLaserHitEffect(FVector InEndLocation)
{
	if (LaserHitParticleEffect)
	{
		const FVector Scale = FVector(LaserHitWidth, LaserHitWidth, LaserHitWidth);
		LaserHitParticleComponent = UGameplayStatics::SpawnEmitterAttached(
			LaserHitParticleEffect, Cast<AACCharacterBoss>(GetOwner())->WheelComponent, 
			AttachPoint, 
			FVector(0.f, 0.f, 0.f), 
			FRotator(0.f, 90.f, 0.f), 
			Scale, 
			EAttachLocation::KeepRelativeOffset, 
			true, 
			EPSCPoolMethod::AutoRelease, 
			true);
		LaserHitParticleComponent->SetWorldLocation(InEndLocation);
		LaserHitParticleComponent->Activate();
	}
}


void UACBossAttackLaserComponent::UpdateLaserEffect()
{
	if (LaserParticleComponent)
	{
		LaserParticleComponent->SetBeamSourcePoint(0, StartLocation, 0);
		LaserParticleComponent->SetBeamTargetPoint(0, EndLocation, 0);
	}
}

void UACBossAttackLaserComponent::EndAttack()
{
	SetActiveLaser(false);
	LaserHitParticleComponent->Deactivate();
	LaserParticleComponent->Deactivate();

	if (CurrentLaserAttackCount > MaxLaserAttackCount)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
}

void UACBossAttackLaserComponent::OnAttack()
{
	if (GetOwner()->HasAuthority())
	{
		if (bIsActiveLaser)
		{
			// EndLocation이 갱신된다. 
			// 클라이언트에서는 OnRep을 통해 UpdateEndLocation이 호출된다.
			// 서버에서는 UpdateEndLocation을 직접 호출해야 한다.			
			UpdateEndLocation(EndLocation);
			OnChangeLaserEndLocation();
			// 서버에서는 히트체크한다.
			CheckHit();
		}
	}
}

void UACBossAttackLaserComponent::UpdateEndLocation(FVector OUT& NewEndLocation)
{
	FRotator LaserRotation = GetOwner()->GetActorRotation();
	LaserRotation.Pitch -= 10.0f;

	NewEndLocation = StartLocation + (LaserRotation.Vector() * AttackRange);
	/*
	if (AACCharacterBoss* Boss = Cast< AACCharacterBoss>(GetOwner()))
	{
		APawn* Target = Boss->GetAttackTarget();

		if (nullptr != Target) 
		{

			const FVector TargetLocation = Target->GetActorLocation();
			FVector Direction = Boss->GetActorRotation().Vector();

			if (FVector::Dist(TargetLocation , Boss->GetActorLocation()) > AttackRange)
			{
				Boss->ChangeTarget();
				Target = Boss->GetAttackTarget();
			}
			
			FVector Offset = Direction * 50.f; // 원하는 거리만큼 벗어나도록 설정 (여기서는 100.0f)
			FVector RandomOffset = FMath::VRand() * 300.0f; // 랜덤 오프셋 추가 (여기서는 50.0f 범위 내에서 랜덤)

			FVector NewEndLocationToTarget = TargetLocation + (Direction)+RandomOffset;

			if (FVector::Dist(NewEndLocationToTarget, Boss->GetActorLocation()) > AttackRange)
			{
				Boss->ChangeTarget();
				return;
			}
			Boss->SetStopRotation(true);
			NewEndLocation = NewEndLocationToTarget;

			// DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), NewEndLocation, FColor::Green, true);
			// DrawDebugSphere(GetWorld(), NewEndLocation, 20.f, 16, FColor::Green, true);
		}
	}*/
}

 
void UACBossAttackLaserComponent::OnRep_EndLocation()
{
	OnChangeLaserEndLocation();
}

// 클라이언트에서 호출된다.
void UACBossAttackLaserComponent::OnRep_bActiveLaser()
{
	OnChangeLaserActive();
}


/*
	클라이언트에서는 OnRep_bActiveLaser함수를 통해 -> OnChangelaserActive 함수가 호출된다.
	서버에서는 OnChangelaserActive 함수가 호출된다.
*/
void UACBossAttackLaserComponent::SetActiveLaser(bool InbIsActiveLaser)
{
	if (bIsActiveLaser == InbIsActiveLaser)
	{
		return;
	}
	if (GetOwner()->HasAuthority()) 
	{
		// bIsActiveLaser이 갱신된다.
		// 클라이언트에서는 OnRep을 통해, OnChangelaserActive가 호출된다.
		bIsActiveLaser = InbIsActiveLaser;
		// 서버에서는 직접 OnChangeLaserActive 함수를 호출한다.
		OnChangeLaserActive();
	}
}

void UACBossAttackLaserComponent::OnChangeLaserActive()
{
	// Laser 공격이 실행된다면
	if (true == bIsActiveLaser) 
	{
		// StartLocation을 지정한다.
		SetStartLocation();
		// 레이저가 생성된다.
		SpawnLaserEffect();
	}
	else 
	{
		// 공격이 종료된다.
		EndAttack();
	}
}

void UACBossAttackLaserComponent::OnChangeLaserEndLocation()
{
	// LaserEffect의 위치가 갱신된다
	UpdateLaserEffect();
}

// 레이저가 생성된다.
void UACBossAttackLaserComponent::SpawnLaserEffect()
{
	if (LaserParticleEffect)
	{
		const FVector Scale = FVector(LaserWidth, LaserWidth, LaserWidth);
		LaserParticleComponent = UGameplayStatics::SpawnEmitterAttached(
			LaserParticleEffect,
			Cast<AACCharacterBoss>(GetOwner())->WheelComponent,
			AttachPoint,
			FVector(0.f, 0.f, 0.f),
			FRotator(0.f, 90.f, 0.f),
			Scale,
			EAttachLocation::KeepRelativeOffset,
			true,
			EPSCPoolMethod::AutoRelease,
			true);
		LaserParticleComponent->Activate();
		bLaserSpawned = true;
	}
}

void UACBossAttackLaserComponent::CheckHit()
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_WorldDynamic, QueryParams);
	if (bHit)
	{
		if (false == bLaserHitSpawned)
		{
			SpawnLaserHitEffect(HitResult.Location);
			bLaserHitSpawned = true;
		}
		else
		{
			if (LaserHitParticleComponent)
			{
				LaserHitParticleComponent->SetWorldLocation(HitResult.Location);
			}
		}

		if (APawn* Player = Cast<AACCharacterPlayer>(HitResult.GetActor()))
		{
			ApplyDamage(Player);
		}
	}
	else
	{
		if (LaserHitParticleComponent)
		{
			LaserHitParticleComponent->Deactivate();
		}
		bLaserHitSpawned = false;
	}
}

void UACBossAttackLaserComponent::SetStartLocation()
{
	StartLocation = Cast<ACharacter>(GetOwner())->GetMesh()->GetSocketLocation(AttachPoint) + LaserStartOffset;
}
