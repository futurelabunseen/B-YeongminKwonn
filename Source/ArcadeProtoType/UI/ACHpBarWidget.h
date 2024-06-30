// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACUserWidget.h"
#include "ACHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACHpBarWidget : public UACUserWidget
{
	GENERATED_BODY()
public:
	UACHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	// FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};
