// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACGamePhase.h"
#include "ACPlayerEntrancePhase.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACPlayerEntrancePhase : public AACGamePhase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<class USoundBase> BackgroundMusic;
public:
	AACPlayerEntrancePhase();
	virtual void OnBeginPhase()override;
	virtual void OnPlayPhase(float DeltaTime)override;
	virtual void OnEndPhase()override;
};
