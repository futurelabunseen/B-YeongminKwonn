// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACUserWidget.h"
#include "ACAmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACAmmoWidget : public UACUserWidget
{
	GENERATED_BODY()
public:
	UACAmmoWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

public:
	void UpdateHPWidget(float CurrentPercent);
protected:
	UPROPERTY()
	TObjectPtr<class UImage> AmmoRadialImage;
	UPROPERTY()
	TObjectPtr<UMaterialInstance> AmmoMaterialInstance;
	TObjectPtr<UMaterialInstanceDynamic> AmmoMaterialInstanceDynamic;
};
