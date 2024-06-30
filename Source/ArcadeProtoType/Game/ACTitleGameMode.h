// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ACTitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACTitleGameMode : public AGameMode
{
	GENERATED_BODY()
	

public:
	AACTitleGameMode();
	void CreateServer();
	void JoinServer(FString IPAddress);

protected:
	virtual void BeginPlay() override;
};
