// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/ACCharacterHUDInterface.h"
#include "ACPlayerState.generated.h"

/**
 * 
 */
UENUM()
enum class EACPlayerState : uint8
{
	None = 0 ,
	Respawning = 1 << 0 ,
	Dead = 1 << 1 ,
	Alive = 1 << 2 
};

DECLARE_MULTICAST_DELEGATE(FOnChangedPlayerStateDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedPlayerStateWidgetDelegate, EACPlayerState);


UCLASS()
class ARCADEPROTOTYPE_API AACPlayerState : public APlayerState,public IACCharacterHUDInterface
{
	GENERATED_BODY()

public:
	AACPlayerState();
	virtual void PostInitializeComponents() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	void SetPlayerState(EACPlayerState IN NewPlayerState);
	FORCEINLINE EACPlayerState GetCurrentPlayerState() const { return CurrentPlayerState; }
	void SetupHUDWidget(class AACCharacterHUD* InHUDWidget);
	FOnChangedPlayerStateDelegate OnChangedPlayerState;
	FOnChangedPlayerStateWidgetDelegate OnChangedPlayerStateWidget;

protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayerState)
	EACPlayerState CurrentPlayerState = EACPlayerState::None;

	UFUNCTION()
	void OnRep_CurrentPlayerState();
	void OnChangeCurrentPlayerState();
};
