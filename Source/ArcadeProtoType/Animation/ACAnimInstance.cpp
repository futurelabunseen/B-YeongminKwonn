#include "Animation/ACAnimInstance.h"
#include "Character/ACCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ArcadeProtoType.h"

UACAnimInstance::UACAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;

	bShouldMove = false;
	CurrentControlType = ECharacterControlType::Default;

	// Animation Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DashActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Arcade/Animation/AnimMontage/AM_Dash.AM_Dash'"));
	if (DashActionMontageRef.Object)
	{
		DashAnimMontage = DashActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Arcade/Animation/AnimMontage/AM_Fire.AM_Fire'"));
	if (FireActionMontageRef.Object)
	{
		FireAnimMontage = FireActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Arcade/Animation/AnimMontage/AM_Dead.AM_Dead'"));
	if (DeadActionMontageRef.Object)
	{
		DeadMontage = DeadActionMontageRef.Object;
	}
}

void UACAnimInstance::PlayDashMontage()
{
	if (!Montage_IsPlaying(DashAnimMontage)) 
	{
		Montage_Play(DashAnimMontage, 1.0f);
	}
}

void UACAnimInstance::PlayAimMotion()
{
	if (ECharacterControlType::Aim == CurrentControlType)
	{
		FRotator Target = UKismetMathLibrary::NormalizedDeltaRotator(Owner->GetBaseAimRotation(), Owner->GetActorRotation());
		AimOffsetRotator.Pitch = FMath::ClampAngle(Target.Pitch, -40, 60);
	}
	else 
	{
		AimOffsetRotator.Pitch = 0;
	}

	AimOffsetRotator.Yaw = 0.f;
	AimOffsetRotator.Roll = 0.f;
}

void UACAnimInstance::PlayDeadMontage()
{
	if (false == Montage_IsPlaying(DeadMontage))
	{
		Montage_Play(DeadMontage, 1.0f);
	}
}

void UACAnimInstance::PlayDefaultAim()
{
	AimOffsetRotator.Pitch = 0;
}

void UACAnimInstance::PlayFireMontage()
{
	if (false == Montage_IsPlaying(FireAnimMontage))
	{
		Montage_Play(FireAnimMontage, 1.0f);
	}
}

void UACAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Owner = Cast<ACharacter>(GetOwningActor()))
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UACAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		bShouldMove = GroundSpeed > 3.0f ? true : false;
	}

	PlayAimMotion();
}


void UACAnimInstance::CheckHainging()
{
	if (nullptr == Owner)
		return;

	if (Owner->GetVelocity().Z >= 0.f || true == Owner->GetMovementComponent()->IsMovingOnGround())//|| false == GetCharacterMovement()->IsFalling())
	{
		return;
	}

	const FVector CurrentLocation = Owner->GetActorLocation();
	const UWorld* currentWorld = GetWorld();

	if (currentWorld)
	{
		FVector LineStart = CurrentLocation + Owner->GetActorForwardVector() * 50.f;
		FVector LineEnd = LineStart - FVector(0.f, 0.f, 50.f);

		// DrawDebugLine(currentWorld, LineStart, LineEnd, FColor::Red, false, 1.0f);
		struct FHitResult OutHit;

		FCollisionQueryParams collisionParams;
		collisionParams.AddIgnoredActor(Owner);

		if (currentWorld->LineTraceSingleByChannel(
			OutHit,
			LineStart,
			LineEnd,
			ECC_Visibility,
			collisionParams))
		{
			LineEnd = OutHit.Location - FVector(0.f, 0.f, 10.f);
			LineStart = FVector(CurrentLocation.X, CurrentLocation.Y, LineEnd.Z);

			// DrawDebugLine(currentWorld, LineStart, LineEnd, FColor::Green, false, 1.0f);
			if (currentWorld->LineTraceSingleByChannel(
				OutHit,
				LineStart,
				LineEnd,
				ECC_Visibility,
				collisionParams))
			{
				FRotator NewRotator = FRotationMatrix::MakeFromX(OutHit.ImpactNormal * (-1)).Rotator();
				Owner->SetActorLocationAndRotation(OutHit.Location + OutHit.ImpactNormal * 50.f, NewRotator);
				Owner->GetCharacterMovement()->GravityScale = 0.f;
				Owner->GetCharacterMovement()->StopMovementImmediately();

				bIsHanging = true;
			}
		}
	}
}

void UACAnimInstance::SetControlType(ECharacterControlType InControlType)
{
	CurrentControlType = InControlType;
}

void UACAnimInstance::AnimNotify_EndDash()
{
	if (OnEndDashEvent.IsBound()) 
	{
		OnEndDashEvent.Broadcast();
	}
}

void UACAnimInstance::AnimNotify_EndDead()
{
	if (OnEndDeadEvent.IsBound()) 
	{
		OnEndDeadEvent.Broadcast();
	}
}
