// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACGamePhase.h"
#include "ACGameOverPhase.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACGameOverPhase : public AACGamePhase
{
	GENERATED_BODY()
	
public:
	AACGameOverPhase();
	virtual void OnBeginPhase()override;
	virtual void OnPlayPhase(float DeltaTime)override;
	virtual void OnEndPhase()override;
};
