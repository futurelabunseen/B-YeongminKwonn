
#include "Character/ACCharacterPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/ACCharacterControlData.h"
#include "Data/ACCharacterRoleTypeData.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "DrawDebugHelpers.h"
#include "Animation/ACAnimInstance.h" 

#include "Kismet/GameplayStatics.h"
#include "Manager/ACDataManager.h"

#include "Components/ACShootingComponent.h"
#include "ArcadePrototype.h"

#include "Player/ACLocalPlayerProxy.h"
#include "Engine/DemoNetDriver.h"
#include "Game/ACGameState.h"
#include "Game/ACPlayerState.h"
#include "EngineUtils.h"

#include "Stat/ACCharacterStatComponent.h"
#include "UI/ACHUDWidget.h"
#include "HUD/ACCharacterHUD.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "ArcadeProtoType.h"
#include "Game/ACArcadeGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"

#include "Manager/ACSpawnManager.h"
#include "Components/CapsuleComponent.h"

AACCharacterPlayer::AACCharacterPlayer()
{
	// Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Arcade/CharacterAssets/Meshes/SM_Character1.SM_Character1'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	

	// InputAction
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDefaultMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Arcade/Input/Actions/IA_DefaultMove.IA_DefaultMove'"));
	if (nullptr != InputActionDefaultMoveRef.Object)
	{
		DefaultMoveAction = InputActionDefaultMoveRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDefaultLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Arcade/Input/Actions/IA_DefaultLook.IA_DefaultLook'"));
	if (nullptr != InputActionDefaultLookRef.Object)
	{
		DefaultLookAction = InputActionDefaultLookRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAimLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Arcade/Input/Actions/IA_AimLook.IA_AimLook'"));
	if (nullptr != InputActionAimLookRef.Object)
	{
		AimLookAction = InputActionAimLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Arcade/Input/Actions/IA_DefaultJump.IA_DefaultJump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDashRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Arcade/Input/Actions/IA_Dash.IA_Dash'"));
	if (nullptr != InputActionDashRef.Object)
	{
		DashAction = InputActionDashRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionZoomInRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Arcade/Input/Actions/IA_ZoomIn.IA_ZoomIn'"));
	if (nullptr != InputActionZoomInRef.Object)
	{
		ZoomInAction = InputActionZoomInRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionZoomOutRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Arcade/Input/Actions/IA_ZoomOut.IA_ZoomOut'"));
	if (nullptr != InputActionZoomOutRef.Object)
	{
		ZoomOutAction = InputActionZoomOutRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAimRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Arcade/Input/Actions/IA_Aim.IA_Aim'"));
	if (nullptr != InputActionAimRef.Object)
	{
		AimAction = InputActionAimRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShootRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Arcade/Input/Actions/IA_Shoot.IA_Shoot'"));
	if (nullptr != InputActionShootRef.Object)
	{
		ShootAction = InputActionShootRef.Object;
	}
	
	// CharacterControl DataAsset
	static ConstructorHelpers::FObjectFinder<UACCharacterControlData> DefaultDataRef(TEXT("/Script/ArcadeProtoType.ACCharacterControlData'/Game/Arcade/Data/DataAsset/ACC_Default.ACC_Default'"));
	if (DefaultDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Default, DefaultDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UACCharacterControlData> AimDataRef(TEXT("/Script/ArcadeProtoType.ACCharacterControlData'/Game/Arcade/Data/DataAsset/ACC_Aim.ACC_Aim'"));
	if (AimDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Aim, AimDataRef.Object);
	}

	// Material
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> EyeMaterialInstanceRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Arcade/CharacterAssets/Materials/MI_AnimMan_Eyes.MI_AnimMan_Eyes'"));
	if (EyeMaterialInstanceRef.Object)
	{
		EyeMaterialInstance = EyeMaterialInstanceRef.Object;
	}
	

	// DeadEffect Material
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstanceRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Arcade/Material/MI_DeadCharacterEffect.MI_DeadCharacterEffect'"));
	if (MaterialInstanceRef.Succeeded())
	{
		DeadMaterialInstance = MaterialInstanceRef.Object;
	}
	// ShootingComponent->SetIsReplicated(true);
	ShootingComponent = CreateDefaultSubobject<UACShootingComponent>("ShootingComponent");
	CurrentCharacterControlType = ECharacterControlType::Default;
	
	// CharacterType
	CharacterType = ECharacterType::eMantus;
}



void AACCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	ResetMaterial();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	if (UACAnimInstance* AnimInstace = Cast<UACAnimInstance>(GetMesh()->GetAnimInstance())) 
	{
		AnimInstace->OnEndDashEvent.AddUObject(this, &AACCharacterPlayer::StopDash);
		AnimInstace->OnEndDeadEvent.AddUObject(this, &AACCharacterPlayer::EndDeadEvent);
	}

	SetCharacterControl(CurrentCharacterControlType);
	// CharacterStat
	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		FACCharacterStat CharacterTypeData = DataManager->GetCharacterStat(CharacterType);
		MaxDashDistance = CharacterTypeData.MaxDashDistance;
	}
	
	if (AACPlayerState* PS = Cast<AACPlayerState>(GetPlayerState()))
	{
		PS->SetPlayerState(EACPlayerState::Alive);
	}

	ShootingComponent->SpawnWeapon();

	// for (APlayerController* ACPlayerController : TActorRange<APlayerController>(GetWorld()))
	// {
	// 	if(HasAuthority())
	// 	{
	// 		if (ACPlayerController)
	// 		{
	// 			AC_LOG(LogACNetwork, Log, TEXT("[%d] Name : %s"), index, *ACPlayerController->GetName());
	// 			++index;
	// 		}
	// 	}
	// }

	// 	CreateGameSession();

	// if (AACGameState* GameState = Cast<AACGameState>(UGameplayStatics::GetGameState(GetWorld())))
	// {
	// 	if (true == GameState->IsSplitScreen())
	// 	{
	// 
	// 		// 나 클라이언트인데, 내 캐릭터를 생성해서 시작할때,
	// 		if (GetNetMode() == NM_Client &&
	// 			GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	// 		{
	// 			// @TODO: 여기 타이밍을 어케 할건지 (게임룸에서 인게임으로 진입할 때?)
	// 
	// 			// 클라 캐릭터 생성됨.
	// 			UE_LOG(LogTemp, Log, TEXT("[ATempProject2Character::BeginPlay()] Client call"));
	// 
	// 			const int32 TEMP_PLATFORM_ID = 1;
	// 			FString ErrorMsg;
	// 			// ULocalPlayerProxy: 네트워크상 플레이어를 대신하는 프록시. LocalPlayer를 상속받고 구현부는 아직 없음.
	// 			UACLocalPlayerProxy* LocalPlayerServerProxy = CreateLocalPlayerProxy(TEMP_PLATFORM_ID, ErrorMsg, true);
	// 
	// 			if (LocalPlayerServerProxy)
	// 			{
	// 				UE_LOG(LogTemp, Log, TEXT("[ATempProject2Character::BeginPlay()] LocalPlayerServerProxy Created"));
	// 			}
	// 			else {
	// 				UE_LOG(LogTemp, Log, TEXT("[ATempProject2Character::BeginPlay()] LocalPlayerServerProxy Failed. Reason: [%s]"), *ErrorMsg);
	// 			}
	// 		}
	// 
	// 		// 나 리슨인데, 이 캐릭터가 내 캐릭터일 때.
	// 		else if (GetNetMode() == NM_ListenServer &&
	// 			GetController() == GetWorld()->GetFirstPlayerController())	// -> 리슨의 컨트롤러 확정.
	// 		{
	// 			UE_LOG(LogTemp, Log, TEXT("[ATempProject2Character::BeginPlay()] Listen call"))
	// 				// 리슨서버에서도 프록시 위처럼 생성해서 줘야함.
	// 				// @TODO: 여기 타이밍을 어케 할건지 (게임룸에서 인게임으로 진입할 때?)
	// 		}
	// 	}
	// }
}

bool AACCharacterPlayer::IsAiming() const
{
	return CurrentCharacterControlType == ECharacterControlType::Aim ? true : false;
}

void AACCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(DefaultMoveAction, ETriggerEvent::Triggered, this, &AACCharacterPlayer::DefaultMove);
	EnhancedInputComponent->BindAction(DefaultLookAction, ETriggerEvent::Triggered, this, &AACCharacterPlayer::DefaultLook);
	EnhancedInputComponent->BindAction(AimLookAction, ETriggerEvent::Triggered, this, &AACCharacterPlayer::AimLook);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AACCharacterPlayer::Dash);

	EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Triggered, this, &AACCharacterPlayer::ZoomIn);
	EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Triggered, this, &AACCharacterPlayer::ZoomOut);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Ongoing, this, &AACCharacterPlayer::OngoingAim);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AACCharacterPlayer::CompletedAim);

}
/*
void ABlasterCharacter::Elim(bool bPlayerLeftGame)
{
	DropOrDestroyWeapons();
	MulticastElim(bPlayerLeftGame);
}
*/

void AACCharacterPlayer::ServerRPCLeaveGame_Implementation()
{
	if (AACArcadeGameMode* ArcadeGameMode = GetWorld()->GetAuthGameMode<AACArcadeGameMode>()) 
	{
		if (AACPlayerState* ArcadePlayerState = GetPlayerState< AACPlayerState>()) 
		{
			ArcadeGameMode->PlayerLeftGame(ArcadePlayerState);
		}
	}
}

void AACCharacterPlayer::Elim(bool bPlayerLeftGame)
{
	MulticastRPCElim(bPlayerLeftGame);
}

void AACCharacterPlayer::MulticastRPCElim_Implementation(bool bPlayerLeftGame)
{
	bLeftGame = bPlayerLeftGame;

	// 
	if (ShootingComponent)
	{
		ShootingComponent->ResetWeapon();
	}
	bElimmed = true;

	// PlayElimMontage();
	
	// Start dissolve effect
	// if (DissolveMaterialInstance)
	// {
	// 	DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
	// 	GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
	// 	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), 0.55f);
	// 	DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Glow"), 200.f);
	// }
	// StartDissolve();



	// Disable character movement
	GetCharacterMovement()->DisableMovement();

	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DisablePlayerInput();
	// Spawn elim bot
	// if (ElimBotEffect)
	// {
	// 	FVector ElimBotSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200.f);
	// 	ElimBotComponent = UGameplayStatics::SpawnEmitterAtLocation(
	// 		GetWorld(),
	// 		ElimBotEffect,
	// 		ElimBotSpawnPoint,
	// 		GetActorRotation()
	// 	);
	// }
// if (ElimBotSound)
// {
// 	UGameplayStatics::SpawnSoundAtLocation(
// 		this,
// 		ElimBotSound,
// 		GetActorLocation()
// 	);
// }
	// bool bHideSniperScope = IsLocallyControlled() &&
	// 	Combat &&
	// 	Combat->bAiming &&
	// 	Combat->EquippedWeapon &&
	// 	Combat->EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle;
	// if (bHideSniperScope)
	// {
	// 	ShowSniperScopeWidget(false);
	// }
	// if (CrownComponent)
	// {
	// 	CrownComponent->DestroyComponent();
	// }
	FTimerHandle ElimTimer;
	const float ElimDelay = 2.f;
	GetWorldTimerManager().SetTimer(
	 	ElimTimer,
	 	this,
	 	&AACCharacterPlayer::ElimTimerFinished,
	 	ElimDelay
	 );
}

void AACCharacterPlayer::ElimTimerFinished()
{
	if (IsLocallyControlled())
	{
		if (OnLeftGame.IsBound()) 
		{
			OnLeftGame.Broadcast();
		}
	}
}

void AACCharacterPlayer::SetupHUDWidget(AACCharacterHUD* InHUDWidget)
{
	if (InHUDWidget) 
	{
		InHUDWidget->UpdateHpWidget(Stat->GetCurrentHp(), Stat->GetCharacterStat().MaxHp);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &AACCharacterHUD::UpdateHpWidget);
		
		ShootingComponent->OnAimStatusChanged.AddUObject(InHUDWidget, &AACCharacterHUD::UpdateAimStatus);
		
		InHUDWidget->UpdateAmmoWidget(100, 100);


		InHUDWidget->UpdateRespawnWidget(100.f,RespawnTime);
		OnRespawnTimeChanged.AddUObject(InHUDWidget,&AACCharacterHUD::UpdateRespawnWidget);
		// if (AACPlayerState* CurrentPlayerState = Cast<AACPlayerState>(GetPlayerState()) )
		// {
		// 	InHUDWidget->UpdateHUDPlayerState(CurrentPlayerState->GetCurrentPlayerState());
		// 	CurrentPlayerState->OnChangedPlayerStateWidget.AddUObject(InHUDWidget, &AACCharacterHUD::UpdateHUDPlayerState);
		// }
	}

	
}


// ========================= SetDead =========================

void AACCharacterPlayer::DisablePlayerInput()
{
	if (IsLocallyControlled())
	{
		if (APlayerController* ArcadePlayerController = Cast<APlayerController>(GetController()))
		{
			GetCharacterMovement()->Velocity = FVector::ZeroVector; // 이동 벡터 초기화
			GetCharacterMovement()->StopMovementImmediately(); // 이동 중지
			ArcadePlayerController->SetIgnoreMoveInput(true);
			ArcadePlayerController->SetIgnoreLookInput(true);
			DisableInput(ArcadePlayerController);
			//FInputModeUIOnly UIOnlyInputMode;
			//ArcadePlayerController->SetInputMode(UIOnlyInputMode);
		}
	}
}

void AACCharacterPlayer::EnablePlayerInput()
{
	if (IsLocallyControlled() && GetController())
	{
		if (APlayerController* ArcadePlayerController = Cast<APlayerController>(GetController()))
		{
			EnableInput(ArcadePlayerController);
			//FInputModeGameOnly GameOnlyInputMode;
			//ArcadePlayerController->SetInputMode(GameOnlyInputMode);
			ArcadePlayerController->SetIgnoreMoveInput(false);
			ArcadePlayerController->SetIgnoreLookInput(false);
			
			// 입력 바인딩 재설정
			SetupPlayerInputComponent(InputComponent); 
		}
	}
}

void AACCharacterPlayer::SetDead()
{
	Super::SetDead();

	if (AACPlayerState* PS = Cast<AACPlayerState>(GetPlayerState()))
	{
		PS->SetPlayerState(EACPlayerState::Dead);
	}
	
	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		if (true == DataManager->IsCharacterRespawnDataValid())
		{
			FACCharacterRespawnData CharacterRespawnData = DataManager->GetCharacterRespawnData(0);
			RemainingRespawnTime = CharacterRespawnData.MaxRespawnTime;
			RespawnTime = CharacterRespawnData.MaxRespawnTime;
		}
	}

	if (ShootingComponent)
	{
		ShootingComponent->EndFire();
	}

	DisablePlayerInput();
}


// Animation Event에서 재생
void AACCharacterPlayer::EndDeadEvent()
{
	if (AACPlayerState* PS = Cast<AACPlayerState>(GetPlayerState()))
	{
		PS->SetPlayerState(EACPlayerState::Respawning);
	}

	// World에서 감추기
	SetActorHiddenInGame(true);
	ShootingComponent->SetWeaponHiddenInGame(true);

	if (UACDataManager* DataManager = UGameInstance::GetSubsystem< UACDataManager>(GetWorld()->GetGameInstance()))
	{
		if (true == DataManager->IsCharacterRespawnDataValid())
		{
			FACCharacterRespawnData CharacterRespawnData = DataManager->GetCharacterRespawnData(0);

			// Respawn Timer
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AACCharacterPlayer::RespawnPlayer, CharacterRespawnData.MaxRespawnTime, false);

			// Respawn Time UI 표시 Timer
			GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &AACCharacterPlayer::UpdateRespawnTime, CharacterRespawnData.RespawnTimerRate, true);
		}
	}
}

void AACCharacterPlayer::UpdateRespawnTime()
{
	if (true == bIsDead)
	{
		RemainingRespawnTime = GetWorld()->GetTimerManager().GetTimerRemaining(RespawnTimerHandle);
		if (OnRespawnTimeChanged.IsBound()) 
		{
			OnRespawnTimeChanged.Broadcast(RemainingRespawnTime, RespawnTime);
		}
	}
}

void AACCharacterPlayer::RespawnPlayer()
{
	if (AACPlayerState* PS = Cast<AACPlayerState>(GetPlayerState()))
	{
		PS->SetPlayerState(EACPlayerState::Alive);
	}

	SetActorHiddenInGame(false);
	ShootingComponent->SetWeaponHiddenInGame(false);
	ResetPlayer();
	SetActorEnableCollision(true);
	EnablePlayerInput();
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

	if (UACSpawnManager* SpawnManager = GetWorld()->GetSubsystem<UACSpawnManager>())
	{
		if (AACGameState* ArcadeGameState = Cast<AACGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			SpawnManager->RequestRespawnCharacter(this, ArcadeGameState->GetCurrentGamePhase());
		}
	}

	GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

void AACCharacterPlayer::ResetPlayer() 
{
	bIsDead = false;
	// HP를 Reset한다.
	Stat->Reset();
	// Ammo Count를 Reset 한다.
	ShootingComponent->ResetWeapon();
	//Material을 Reset 한다.
	ResetMaterial();
	// Collision을
	
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AACCharacterPlayer::ResetMaterial()
{
	BodyMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(BodyMaterialInstance, this);
	for (int MaterialIndex = 0; MaterialIndex < 15; ++MaterialIndex)
	{
		GetMesh()->SetMaterial(MaterialIndex, BodyMaterialInstanceDynamic);
	}
}

/// ======================================================= ////

void AACCharacterPlayer::UpdateMaterialDissolve(float DeltaTime)
{
	const float ChangeValue = -1.f;
	DissolveValue = FMath::FInterpTo(DissolveValue, ChangeValue, DeltaTime, InterpSpeed);
	if (DeadMaterialInstanceDynamic)
	{
		DeadMaterialInstanceDynamic->SetScalarParameterValue(FName("Dissolve"), DissolveValue);
		for (int i = 0; i < 15; ++i) 
		{
			GetMesh()->SetMaterial(i, DeadMaterialInstanceDynamic);
		}
	}
	else 
	{
		DeadMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(DeadMaterialInstance, this);
	}
}

FVector AACCharacterPlayer::GetCameraForwardDirection() const
{
	return FollowCamera->GetForwardVector();
}


///////////////////////////

void AACCharacterPlayer::DefaultMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AACCharacterPlayer::DefaultLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AACCharacterPlayer::AimLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>() * 0.5f;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
// ========================= Aim =========================


void AACCharacterPlayer::OngoingAim()	
{
	if (IsLocallyControlled())
	{
		SetCharacterControl(ECharacterControlType::Aim);

		if (UACAnimInstance* AnimInstace = Cast<UACAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstace->SetControlType(CurrentCharacterControlType);
		}	
	}
	if (ShootingComponent->OnAimStatusChanged.IsBound()) 
	{
		ShootingComponent->OnAimStatusChanged.Broadcast(EAimType::AIM);
	}
}

void AACCharacterPlayer::CompletedAim()
{
	if (IsLocallyControlled())
	{
		SetCharacterControl(ECharacterControlType::Default);

		if (UACAnimInstance* AnimInstace = Cast<UACAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstace->SetControlType(CurrentCharacterControlType);
		}
	}
	if (ShootingComponent->OnAimStatusChanged.IsBound()) 
	{
		ShootingComponent->OnAimStatusChanged.Broadcast(EAimType::DEFAULT);
	}
}

void AACCharacterPlayer::MulticastRPCPlayAimAnimation_Implementation() 
{
	if (false == IsLocallyControlled())
	{
		if (UACAnimInstance* AnimInstace = Cast<UACAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstace->SetControlType(CurrentCharacterControlType);
		}
	}
}


void AACCharacterPlayer::ClientRPCPlayAimAnimation_Implementation(AACCharacterPlayer* Player)
{
	if (Player) 
	{
		if (UACAnimInstance* AnimInstance = Cast<UACAnimInstance>(Player->GetMesh()->GetAnimInstance()))
		{
			AnimInstance->SetControlType(CurrentCharacterControlType);
		}
	}
}


void AACCharacterPlayer::ZoomIn()
{
	const float MinTargetArmLength = CharacterControlManager[CurrentCharacterControlType]->MinTargetArmLength;
	float TargetArmLength = CameraBoom->TargetArmLength;

	CameraBoom->TargetArmLength = MinTargetArmLength < TargetArmLength ? TargetArmLength - 20.f : TargetArmLength;
}

void AACCharacterPlayer::ZoomOut()
{
	const float MaxTargetArmLength = CharacterControlManager[CurrentCharacterControlType]->MaxTargetArmLength;
	float TargetArmLength = CameraBoom->TargetArmLength;

	CameraBoom->TargetArmLength = MaxTargetArmLength > TargetArmLength ? TargetArmLength + 20.f : TargetArmLength;
}


void AACCharacterPlayer::ServerRPCSetMaxWalkSpeed_Implementation(float InMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InMaxWalkSpeed;
}

void AACCharacterPlayer::SetCharacterControlData(const UACCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
	ServerRPCSetMaxWalkSpeed(CharacterControlData->WalkMaxSpeed);

	// CameraBoom
	CameraBoom->TargetArmLength = CharacterControlManager[ECharacterControlType::Default]->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->SetRelativeLocation(CharacterControlData->RelativeLocation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}


// ========================= Shoot =========================
void AACCharacterPlayer::Shoot()
{
	if (ECharacterControlType::Aim == CurrentCharacterControlType)
	{
		if (false == HasAuthority()) 
		{
			PlayShootAnimation();
		}

		ServerRPCPlayShootAnimation();
	}
}

void AACCharacterPlayer::PlayShootAnimation()
{
	// Play Frie AnimMontage
	if (UACAnimInstance* AnimInstance = Cast<UACAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->PlayFireMontage();
	}
}

void AACCharacterPlayer::ServerRPCPlayShootAnimation_Implementation()
{
	PlayShootAnimation();

	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				AACCharacterPlayer* OtherPlayer = Cast<AACCharacterPlayer>(PlayerController->GetPawn());
				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCPlayShootAnimation(this);
				}
			}
		}
	}
}

void AACCharacterPlayer::ClientRPCPlayShootAnimation_Implementation(AACCharacterPlayer* Player)
{
	Player->PlayShootAnimation();
}

// ========================= Dash =========================

void AACCharacterPlayer::PlayDashAnimation()
{
	if (UACAnimInstance* AnimInstance = Cast<UACAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->PlayDashMontage();
	}
}

void AACCharacterPlayer::Dash()
{
	if (false == bIsDashing)
	{
		bIsDashing = true;

		const FVector ForwardVector = GetActorRotation().Vector();
		const FVector ImpulseValue = ForwardVector * MaxDashDistance;

		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		GetCharacterMovement()->AddImpulse(ImpulseValue, true);
		if (HasAuthority()) 
		{
			PlayDashAnimation();
		}
		else
		{
			ServerRPCDash();
		}
	}
}

void AACCharacterPlayer::ServerRPCDash_Implementation()
{
	if (false == bIsDashing) 
	{
		bIsDashing = true;

		const FVector ForwardVector = GetActorRotation().Vector();
		const FVector ImpulseValue = ForwardVector * MaxDashDistance;

		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		GetCharacterMovement()->AddImpulse(ImpulseValue, true);

		PlayDashAnimation();

		for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
		{
			if (PlayerController && GetController() != PlayerController)
			{
				if (!PlayerController->IsLocalController())
				{
					AACCharacterPlayer* OtherPlayer = Cast<AACCharacterPlayer>(PlayerController->GetPawn());
					if (OtherPlayer)
					{
						OtherPlayer->ClientRPCPlayDashAnimation(this);
					}
				}
			}
		}
	}
}

void AACCharacterPlayer::ClientRPCPlayDashAnimation_Implementation(AACCharacterPlayer* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayDashAnimation();
	}
}

void AACCharacterPlayer::StopDash()
{
	bIsDashing = false;

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}

void AACCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	CurrentCharacterControlType = NewCharacterControlType;

	if (false == IsLocallyControlled())
	{
		return;
	}

	UACCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);
	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
}

UACLocalPlayerProxy* AACCharacterPlayer::CreateLocalPlayerProxy(int32 ControllerId, FString& OutError, bool bSpawnPlayerController)
{
	// TODO: 여기 값 어케해야하는지 알아봐야함. 진짜 필요없나?
	const int32 TEMP_PLATFORM_USER_ID_FOR_INTERNAL = -1;
	FPlatformUserId UserId = FPlatformUserId::CreateFromInternalId(TEMP_PLATFORM_USER_ID_FOR_INTERNAL);
	return CreateLocalPlayerProxy(UserId, OutError, bSpawnPlayerController);
}

UACLocalPlayerProxy* AACCharacterPlayer::CreateLocalPlayerProxy(FPlatformUserId UserId, FString& OutError, bool bSpawnPlayerController)
{
	return nullptr;
	/*
	// @ From GameInstance
	check(GetGameInstance()->GetEngine()->LocalPlayerClass != nullptr);

	UACLocalPlayerProxy* NewPlayer = nullptr;
	
	int32 InsertIndex = INDEX_NONE;
	UGameViewportClient* GameViewport = GetGameInstance()->GetGameViewportClient();

	if (GameViewport == nullptr)
	{
		if (ensure(GetGameInstance()->IsDedicatedServerInstance()))
		{
			OutError = FString::Printf(TEXT("Dedicated servers cannot have local players"));
			return nullptr;
		}
	}
	
	const int32 MaxSplitscreenPlayers = GameViewport ? GameViewport->MaxSplitscreenPlayers : 1;

	if (GetGameInstance()->FindLocalPlayerFromPlatformUserId(UserId) != NULL)
	{
		OutError = FString::Printf(TEXT("A local player already exists for PlatformUserId %d,"), UserId.GetInternalId());
	}
	else if (GetGameInstance()->GetNumLocalPlayers() < MaxSplitscreenPlayers)
	{
		// If the controller ID is not specified then find the first available
		if (!UserId.IsValid())
		{
			for (int32 Id = 0; Id < MaxSplitscreenPlayers; ++Id)
			{
				// Iterate until we find a null player. We want the next available platform user ID
				//FPlatformUserId DummyId = IPlatformInputDeviceMapper::Get().GetPlatformUserForUserIndex(Id);

				// TODO: 여기 값 어케해야하는지 알아봐야함. 진짜 필요없나?
				FPlatformUserId DummyId = FPlatformUserId::CreateFromInternalId(-1);

				if (DummyId.IsValid())
				{
					UserId = DummyId;
				}

				if (GetGameInstance()->FindLocalPlayerFromControllerId(Id) == nullptr)
				{
					break;
				}
			}
			check(UserId.GetInternalId() < MaxSplitscreenPlayers);
		}
		else if (UserId.GetInternalId() >= MaxSplitscreenPlayers)
		{
			UE_LOG(LogPlayerManagement, Warning, TEXT("Controller ID (%d) is unlikely to map to any physical device, so this player will not receive input"), UserId.GetInternalId());
		}

		NewPlayer = NewObject<UACLocalPlayerProxy>(GEngine, UACLocalPlayerProxy::StaticClass(), TEXT("LocalPlayerProxy"), EObjectFlags::RF_Dynamic);
		InsertIndex = GetGameInstance()->AddLocalPlayer(NewPlayer, UserId);
		UWorld* CurrentWorld = GetWorld();
		if (bSpawnPlayerController && InsertIndex != INDEX_NONE && CurrentWorld != nullptr)
		{
			if (CurrentWorld->GetNetMode() != NM_Client)
			{
				// server; spawn a new PlayerController immediately
				if (!NewPlayer->SpawnPlayActor("", OutError, CurrentWorld))
				{
					GetGameInstance()->RemoveLocalPlayer(NewPlayer);
					NewPlayer = nullptr;
				}
			}
			else if (CurrentWorld->IsPlayingReplay())
			{
				if (UDemoNetDriver* DemoNetDriver = CurrentWorld->GetDemoNetDriver())
				{
					// demo playback; ask the replay driver to spawn a splitscreen client
					if (false == DemoNetDriver->SpawnSplitscreenViewer(NewPlayer, CurrentWorld))
					{
						GetGameInstance()->RemoveLocalPlayer(NewPlayer);
						NewPlayer = nullptr;
					}
				}
			}

			// 
			// 			if (UDemoNetDriver* DemoNetDriver = CurrentWorld->GetDemoNetDriver())
			// {
			// 	// demo playback; ask the replay driver to spawn a splitscreen client
			// 	if (!DemoNetDriver->SpawnSplitscreenViewer(NewPlayer, CurrentWorld))
			// 	{
			// 		RemoveLocalPlayer(NewPlayer);
			// 		NewPlayer = nullptr;
			// 	}
			// }
	/*
			else
			{
				// client; ask the server to let the new player join
				TArray<FString> Options;
				NewPlayer->SendSplitJoin(Options);
			}
		}
	}
	else
	{
		OutError = FString::Printf(TEXT("Maximum number of players (%d) already created.  Unable to create more."), MaxSplitscreenPlayers);
	}

	if (OutError != TEXT(""))
	{
		UE_LOG(LogPlayerManagement, Log, TEXT("UPlayer* creation failed with error: %s"), *OutError);
	}

	return NewPlayer; */
}

// Session


// void AACCharacterPlayer::OnRep_CharacterControlType()
// {
// 	if (UACAnimInstance* AnimInstace = Cast<UACAnimInstance>(GetMesh()->GetAnimInstance()))
// 	{
// 		AnimInstace->SetControlType(CurrentCharacterControlType);
// 	}
// }

