#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ACMinionAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(OnAttackDelegate);

/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API UACMinionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UACMinionAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackAnimMontage;

public:
	OnAttackDelegate OnEndAttackEvent;
	OnAttackDelegate OnStartAttackEvent;

	void PlayAttackMontage();
	UFUNCTION()
	void AnimNotify_EndAttack();
	UFUNCTION()
	void AnimNotify_StartAttack();
};
