// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACUserWidget.h"
#include "ACTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACTimerWidget : public UACUserWidget
{
	GENERATED_BODY()
public:
	UACTimerWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

public:
	void UpdateTimerTextWidget(FString InCurrentLeftTime);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentLeftTime;
};
