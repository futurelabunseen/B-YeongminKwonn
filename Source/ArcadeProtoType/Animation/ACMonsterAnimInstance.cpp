#include "Animation/ACMonsterAnimInstance.h"
#include "Character/ACCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UACMonsterAnimInstance::UACMonsterAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;

	// static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Arcade/Animation/AnimMontage/AM_Fire.AM_Fire'"));
	// if (DeadActionMontageRef.Object)
	// {
	// 	DeadMontage = DeadActionMontageRef.Object;
	// }


		// Attack1Montage;
	// static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack1ActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Arcade/Animation/AnimMontage/AM_MonsterAttak1.AM_MonsterAttak1'"));
	// if (Attack1ActionMontageRef.Object)
	// {
	// 	Attack1Montage = Attack1ActionMontageRef.Object;
	// }
	// 
	// 
	// // Attack2Montage;
	// 
	// static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack2ActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Arcade/Animation/AnimMontage/AM_MonsterAttak2.AM_MonsterAttak2'"));
	// if (Attack2ActionMontageRef.Object)
	// {
	// 	Attack2Montage = Attack2ActionMontageRef.Object;
	// }

}

void UACMonsterAnimInstance::PlayAttack1ActionMontage()
{
	//if (false == Montage_IsPlaying(Attack1Montage))
	//{
	//	Montage_Play(Attack1Montage, 1.0f);
	//}
}

void UACMonsterAnimInstance::PlayAttack2ActionMontage()
{
	//if (false == Montage_IsPlaying(Attack2Montage))
	//{
	//	Montage_Play(Attack2Montage, 1.0f);
	//}
}

void UACMonsterAnimInstance::PlayDeadMontage()
{
	//if (false == Montage_IsPlaying(DeadMontage))
	//{
	//	Montage_Play(DeadMontage, 1.0f);
	//}
}

void UACMonsterAnimInstance::NativeInitializeAnimation()
{
	//if (Owner = Cast<ACharacter>(GetOwningActor()))
	//{
	//	Movement = Owner->GetCharacterMovement();
	//}
}

void UACMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//if (Movement)
	//{
	//	Velocity = Movement->Velocity;
	//	GroundSpeed = Velocity.Size2D();
	//	bIsIdle = GroundSpeed < MovingThreshould;
	//
	//}
}
