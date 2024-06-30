#include "TriggerBox/ACPhaseChangerTriggerBox.h"
#include "Components/ShapeComponent.h"
#include "Character/ACCharacterPlayer.h"
#include "Interface/ACGamePhaseInterface.h"

AACPhaseChangerTriggerBox::AACPhaseChangerTriggerBox()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AACPhaseChangerTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AACPhaseChangerTriggerBox::OnActorBeginOverlap);
}

void AACPhaseChangerTriggerBox::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}
	
	if (HasAuthority()) 
	{
		if (IACGamePhaseInterface* GameModePhaseInterface = Cast<IACGamePhaseInterface>(GetWorld()->GetGameState()))
		{
			if (Cast<AACCharacterPlayer>(OtherActor))
			{
				GameModePhaseInterface->SetGamePhase(CurrentBoxPhase);
				Destroy();
			}
		}
	}
}
