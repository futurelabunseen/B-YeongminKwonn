
#include "Character/Components/ACFlyingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/ACCharacterNonPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UACFlyingComponent::UACFlyingComponent()
{
	// PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

}


// Called when the game starts
void UACFlyingComponent::BeginPlay()
{
	Super::BeginPlay();

	MonsterCharacter = Cast<AACCharacterNonPlayer>(GetOwner());

	if (MonsterCharacter)
	{
		CapsuleComponent = MonsterCharacter->GetCapsuleComponent();
		MovementComponent = MonsterCharacter->GetCharacterMovement();

		MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
		MovementComponent = MonsterCharacter->GetCharacterMovement();
		MovementComponent->GravityScale = 0.0f;
	}
}


// Called every frame
void UACFlyingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
