// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(Meta = (CharacterMantus))
	TObjectPtr<class UButton> CharacterMantus_Button;
	UPROPERTY(Meta = (CharacterCollie))
	TObjectPtr<class UButton> CharacterCollie_Button;
	
	UPROPERTY(Meta = (CreateServerButton))
	TObjectPtr<class UButton> CreateServer_Button;
	UPROPERTY(Meta = (JoinServerButton))
	TObjectPtr<class UButton> JoinServer_Button;

	UFUNCTION(BlueprintCallable)
	void CreateServerHandler();
	UFUNCTION(BlueprintCallable)
	void JoinServerHandler();
	UFUNCTION(BlueprintCallable)
	void SelectCharacterMantus();
	UFUNCTION(BlueprintCallable)
	void SelectCharacterCollie();
};
