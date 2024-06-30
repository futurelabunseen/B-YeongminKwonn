// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Game/GamePhase/ACGamePhase.h"
#include "ACPhaseChangerTriggerBox.generated.h"

UCLASS()
class ARCADEPROTOTYPE_API AACPhaseChangerTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	AACPhaseChangerTriggerBox();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EACGamePhaseList CurrentBoxPhase = EACGamePhaseList::MinionCombat;
};
