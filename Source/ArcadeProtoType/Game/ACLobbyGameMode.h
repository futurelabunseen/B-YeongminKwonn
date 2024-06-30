// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ACLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AACLobbyGameMode();
	
	UFUNCTION(BlueprintCallable)
	void CreateServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer();

	virtual void PostLogin(APlayerController* newController)override;
};
