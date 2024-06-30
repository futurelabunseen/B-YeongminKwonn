// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ACUserWidget.h"
#include "ACGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACGameOverWidget : public UACUserWidget
{
	GENERATED_BODY()
	
	public:
		UACGameOverWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateResultText(FString Result);
	void SetupWidget();

	UFUNCTION()
	void ClickMainMenuButton();
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	void OnPlayerLeftGame();
	//
	UFUNCTION()
	void ClickExitButton();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GameResultText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MainMenuButton;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton;

	UPROPERTY()
	TObjectPtr<class APlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<class UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;
};
