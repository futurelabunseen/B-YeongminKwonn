// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACUserWidget.h"
#include "ACAimAssistWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACAimAssistWidget : public UACUserWidget
{
	GENERATED_BODY()
public:
	UACAimAssistWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateAimAssistWidget(bool bIsAiming);
	virtual void NativeConstruct() override;

protected:
	float Recoil = 25.0f;
	TArray<class USizeBox*> SizeBoxArray;
	TArray<FVector2D> DefaultTranslationArray;
	TObjectPtr<class USizeBox> RightBox;
	TObjectPtr<class USizeBox> LeftBox;
	TObjectPtr<class USizeBox> TopBox;
	TObjectPtr<class USizeBox> BottomBox;
};
