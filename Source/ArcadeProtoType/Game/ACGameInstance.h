// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/ACDataUtility.h"
#include "ACGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UACGameInstance();
	virtual void Init() override;
	UFUNCTION()
	void SetCharacterType(ECharacterType InCharacterType);
	ECharacterType GetCharacterType() const;
private:
	UPROPERTY()
	TObjectPtr<class UACDataManager> DataManager;
	UPROPERTY()
	ECharacterType CurrentCharacterType;
};
