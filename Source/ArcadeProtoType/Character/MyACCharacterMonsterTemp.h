// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacterBase.h"
#include "MyACCharacterMonsterTemp.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AMyACCharacterMonsterTemp : public AACCharacterBase
{
	GENERATED_BODY()
public:
	AMyACCharacterMonsterTemp();
	void Dead();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

protected:
	UPROPERTY(EditAnywhere, Category = "MonsterMesh")
	TObjectPtr<UStaticMeshComponent> MonsterMesh;
};
