// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACGamePhase.h"
#include "ACWaitingToStartPhase.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACWaitingToStartPhase : public AACGamePhase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<class USoundBase> BackgroundMusic;

public:
	AACWaitingToStartPhase();
	virtual void OnBeginPhase() override;
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCOnBeginPhase();
	virtual void OnPlayPhase(float DeltaTime) override;
	virtual void OnEndPhase()override;
};
