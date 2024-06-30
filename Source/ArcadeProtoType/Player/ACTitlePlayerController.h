// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACTitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACTitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	AACTitlePlayerController();
	void CreateServer();
	void JoinServer();

protected:
	virtual void BeginPlay() override;
};
