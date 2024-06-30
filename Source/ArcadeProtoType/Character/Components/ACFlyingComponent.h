// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACFlyingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADEPROTOTYPE_API UACFlyingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACFlyingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// virtual void SetInitialize(class UCapsuleComponent* InCapsuleComponent);

	FVector DirectionAlpha;
protected:
	TObjectPtr<class AACCharacterNonPlayer> MonsterCharacter;
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
};
