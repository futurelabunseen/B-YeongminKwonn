// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ACCharacterBase.h"
#include "Data/ACDataUtility.h"
#include "Interface/ACCharacterHUDInterface.h"
// #include "Interfaces/OnlineSessionInterface.h"
#include "InputActionValue.h"
#include "ACCharacterPlayer.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangedRespawnTimeDelegate, float /*CharacterState*/, float /*CharacterState*/);
DECLARE_MULTICAST_DELEGATE(FOnLeftGame);
UCLASS()
class ARCADEPROTOTYPE_API AACCharacterPlayer : public AACCharacterBase, public IACCharacterHUDInterface
{
	GENERATED_BODY()
public:
	AACCharacterPlayer();

public:
	bool IsAiming() const;
	virtual void Tick(float DeltaTime) override; //RemainingRespawnTime 

protected:
	virtual void ResetMaterial() override;
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Dead Section
	virtual void SetDead();
	void EndDeadEvent();
	virtual void RespawnPlayer();
	void UpdateRespawnTime();
	virtual void ResetPlayer();

	//
	void DisablePlayerInput();
	void EnablePlayerInput();

	// Dissolve Speed
	float InterpSpeed = 1.f;
	
	// Respawn
	float RespawnTime = 5.f;
	FTimerHandle RespawnTimerHandle; 
	FTimerHandle UpdateTimerHandle;
	float RemainingRespawnTime = 0.f;
	FOnChangedRespawnTimeDelegate OnRespawnTimeChanged;

	// State
	// ECharacterState CharacterState = ECharacterState::Alive;
	//FOnChangedCharacterStateDelegate OnCharacterStateChanged;

	// UI Section
public:
	UFUNCTION(Server, Reliable)
	void ServerRPCLeaveGame();
	void Elim(bool bPlayerLeftGame);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCElim(bool bPlayerLeftGame);
	void ElimTimerFinished();
	FOnLeftGame OnLeftGame;
	virtual void SetupHUDWidget(class AACCharacterHUD* InHUDWidget) override;
	virtual void UpdateMaterialDissolve(float DeltaTime);
	// FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;}
	FVector GetCameraForwardDirection() const;
	// Input Action

protected:
	uint8  bLeftGame = false;
	uint8 bElimmed = false;
	void DefaultMove(const FInputActionValue& Value);
	void DefaultLook(const FInputActionValue& Value);
	void AimLook(const FInputActionValue& Value);

	void OngoingAim();
	virtual void CompletedAim();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCPlayAimAnimation();

	UFUNCTION(Client, Reliable)
	void ClientRPCPlayAimAnimation(AACCharacterPlayer* Player);

	void ZoomIn();
	void ZoomOut();

	// ======== SHOOT
	virtual void Shoot();
	void PlayShootAnimation();
	UFUNCTION(Server, Unreliable)
	void ServerRPCPlayShootAnimation();
	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayShootAnimation(AACCharacterPlayer* CharacterToPlay);

	// ======== DASH
	void PlayDashAnimation();
	virtual void Dash();
	void StopDash();
	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayDashAnimation(AACCharacterPlayer* CharacterToPlay);
	UFUNCTION(Server, Unreliable)
	void ServerRPCDash();

	UFUNCTION(Server, Reliable)
	void ServerRPCSetMaxWalkSpeed(float InMaxWalkSpeed);

private:
	class UACLocalPlayerProxy* CreateLocalPlayerProxy(int32 ControllerId, FString& OutError, bool bSpawnPlayerController);
	class UACLocalPlayerProxy* CreateLocalPlayerProxy(FPlatformUserId UserId, FString& OutError, bool bSpawnPlayerController);

protected:
	// Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Shooing Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shoot)
	TObjectPtr<class UACShootingComponent> ShootingComponent;
	
	// Dash
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash)
	uint8 bIsDashing : 1;

	// Shooting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shooting)
	uint8 bIsShooting : 1;

	

protected:
	UPROPERTY()
	TObjectPtr<UMaterialInstance> BodyMaterialInstance;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BodyMaterialInstanceDynamic;
	TObjectPtr<UMaterialInstance> EyeMaterialInstance;
	float MaxDashDistance;

	// Input Action
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DefaultMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DefaultLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>  AimLookAction; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomInAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomOutAction;

protected:
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UACCharacterControlData*> CharacterControlManager;
	ECharacterControlType CurrentCharacterControlType;

	virtual void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UACCharacterControlData* CharacterControlData);
};
