// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacterPlayer.h"
#include "ACCharacterCollie.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACCharacterCollie : public AACCharacterPlayer
{
	GENERATED_BODY()
public:
	AACCharacterCollie();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Shoot() override;

};
