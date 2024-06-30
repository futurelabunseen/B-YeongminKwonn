#include "Character/Components/ACFlyingMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UACFlyingMovementComponent::UACFlyingMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UACFlyingMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Cast<ACharacter>(GetOwner())->GetCharacterMovement()->GravityScale = 0.f;
}
