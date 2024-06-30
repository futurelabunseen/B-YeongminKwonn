#include "ACPlayerState.h"
#include "ArcadeProtoType.h"
#include "HUD/ACCharacterHUD.h"
#include "Net/UnrealNetwork.h"

AACPlayerState::AACPlayerState()
{
	CurrentPlayerState = EACPlayerState::Alive;
}

void AACPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void AACPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACPlayerState, CurrentPlayerState);
}

void AACPlayerState::SetPlayerState(EACPlayerState IN NewPlayerState)
{
	if (HasAuthority()) 
	{
		CurrentPlayerState = NewPlayerState;
		OnChangeCurrentPlayerState();
	}
}

void AACPlayerState::SetupHUDWidget(AACCharacterHUD* InHUDWidget)
{
	if (InHUDWidget) 
	{
		OnChangedPlayerStateWidget.AddUObject(InHUDWidget, &AACCharacterHUD::UpdateHUDPlayerState);
	}
}

void AACPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

}

void AACPlayerState::OnRep_CurrentPlayerState()
{
	OnChangeCurrentPlayerState();

}

void AACPlayerState::OnChangeCurrentPlayerState()
{
	 if (GetPlayerController() && GetPlayerController()->IsLocalController())
	 {
	 	if (GetPlayerController()->GetHUD())
	 	{
	 		Cast<AACCharacterHUD>(GetPlayerController()->GetHUD())->UpdateHUDPlayerState(CurrentPlayerState);
			// OnChangedPlayerStateWidget.Broadcast(CurrentPlayerState);
	 	}
	 }
	
	 if (OnChangedPlayerState.IsBound()) 
	 {
		 OnChangedPlayerState.Broadcast();
	 }
}
