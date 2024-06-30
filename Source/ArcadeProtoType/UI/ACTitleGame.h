// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACUserWidget.h"
#include "ACTitleGame.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACTitleGame : public UACUserWidget
{
	GENERATED_BODY()
public:
	UACTitleGame(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void CreateServer(); 
	UFUNCTION(BlueprintCallable)
	void JoinServer();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> HostButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> JoinButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> IPTextBox;
};
