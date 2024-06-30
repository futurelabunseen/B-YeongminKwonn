// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACUserWidget.h"
#include "ACHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACHUDWidget : public UACUserWidget
{
	GENERATED_BODY()
public:
	UACHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

};
