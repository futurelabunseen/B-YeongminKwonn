// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACUserWidget.h"
#include "ACHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACHpWidget : public UACUserWidget
{
	GENERATED_BODY()
public:
	UACHpWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

public:
	void UpdateHPWidget(float CurrentPercent);
	void SetImagePath(const TCHAR* ImagePath);
	void SetImageTexture(UTexture* ImageTexture);
protected:
	UPROPERTY()
	TObjectPtr<class UImage> HpRadialImage;
	UPROPERTY()
	TObjectPtr<class UImage> CharacterProfileImage;
	TObjectPtr<UMaterialInstance> HpMaterialInstance;
	TObjectPtr<UMaterialInstance> ProfileMaterialInstance;
	TObjectPtr<UMaterialInstanceDynamic> HpMaterialInstanceDynamic;
};
