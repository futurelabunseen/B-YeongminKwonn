// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACFlyingMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADEPROTOTYPE_API UACFlyingMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACFlyingMovementComponent();

protected:
	virtual void BeginPlay() override;
		
};
