// Fill ou your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacterPlayer.h"
#include "InputActionValue.h"
#include "ACCharacterMantus.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACCharacterMantus : public AACCharacterPlayer
{
	GENERATED_BODY()
public:
	AACCharacterMantus();
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

protected:
	virtual void Shoot() override;
	virtual void EndShoot();
	virtual void CompletedAim() override;
};
