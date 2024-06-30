// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/ACCharacterHUDInterface.h"
#include "ACPlayerController.generated.h"

// DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTimeChangedDelegate, int32 /*Minite*/, int32 /*Seconds*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChangedDelegate, FString /*TimeText*/);
DECLARE_MULTICAST_DELEGATE(FOnTimerExpiredDelegate);
/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACPlayerController : public APlayerController,public IACCharacterHUDInterface
{
	GENERATED_BODY()
public:
	AACPlayerController();
	virtual void Tick(float DeltaSeconds) override;
	virtual void ReceivedPlayer() override;
	void SetupHUDWidget(class AACCharacterHUD* InHUDWidget);
	// void SetupGameState(class AACGameState* GameState);
	void CreateServer();
	void JoinServer();
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void PostInitializeComponents() override;
public:
	FOnTimeChangedDelegate OnTimeChanged;
	FOnTimerExpiredDelegate OnTimerExpired;
	UFUNCTION()
	void UpdateTime();
	void UpdateTimeText(float Time);

protected:
	FTimerHandle GameTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UACHUDWidget> ACHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UACHUDWidget> ACHUDWidget;

	// UFUNCTION(Client,Unreliable)
	// void ClientRPCGameStart();
	uint8 bIsClinetReady :1 ;


	// Timer
	float RunningForSync = 0.f;
	float TimeSyncRunningTime = 0.f;
	float ClientServerDelta = 0.f;
	int CountdownInt = 0.f;

	float TimerCheckRate = 3.0f;
	float TotalTime = 300.f;

	FString TimeText;

	void SetSyncBetweenServerAndClient(float DeltaTime);

	UFUNCTION(Server, Unreliable)
	void ServerRPCRequestServerTime(float TimeOfClientRequest);

	UFUNCTION(Client, Unreliable)
	void ClientRPCReportServerTime(float TimeOfClientRequest, float TimeOfServer);
	// void SetGameTimeInHUD(float Time);
	float GetServerTime();



};
