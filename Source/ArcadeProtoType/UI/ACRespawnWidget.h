// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACUserWidget.h"
#include "ACRespawnWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACRespawnWidget : public UACUserWidget
{
	GENERATED_BODY()
public:
	UACRespawnWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

public:
	void UpdateWidget(float value);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> RespawnRadialImage;
	TObjectPtr<UMaterialInstance>  RespawnMaterialInstance;
	TObjectPtr<UMaterialInstanceDynamic>  RespawnMaterialInstanceDynamic;
};
