
#include "Game/ACGameMode.h"
#include "Game/ACGameInstance.h"
#include "ACGameMode.h"
#include "ACGameInstance.h"
#include "EngineUtils.h"
#include "ArcadeProtoType.h"
#include "Kismet/GameplayStatics.h"
#include "ACGameState.h"
#include "ACPlayerState.h"
#include "Character/ACCharacterBoss.h"
#include "Character/ACCharacterMinion.h"
#include "GameFramework/HUD.h"

AACGameMode::AACGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> MantusClassRef(TEXT(
		 "/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_CharacterMantus.BP_CharacterMantus_C'"));
	if (MantusClassRef.Class)
	{
		CharacterTypes.Add(ECharacterType::eMantus, MantusClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<APawn> CollieClassRef(TEXT(
	"/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_CharacterCollie.BP_CharacterCollie_C'"));
	if (CollieClassRef.Class)
	{
		CharacterTypes.Add(ECharacterType::eCollie, CollieClassRef.Class);
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArcadeProtoType.ACPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> DefaultCharacterHUDRef(TEXT(
		"/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_ACCharacterHUD.BP_ACCharacterHUD_C'"));
	if (DefaultCharacterHUDRef.Class)
	{
		HUDClass = DefaultCharacterHUDRef.Class;
	}


	static ConstructorHelpers::FClassFinder<AACCharacterBoss> CharacterBossRef(TEXT(
		"/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_ACBoss.BP_ACBoss_C'"));
	if (CharacterBossRef.Class)
	{
		BossClass = CharacterBossRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AACCharacterMinion> MinionClassRef(TEXT(
		"/Script/Engine.Blueprint'/Game/Arcade/Blueprint/BP_ACMinion.BP_ACMinion_C'"));
	if (MinionClassRef.Class)
	{
		MinionClass = MinionClassRef.Class;
	}

	GameStateClass = AACGameState::StaticClass();
	PlayerStateClass = AACPlayerState::StaticClass();

	// DefaultPawnClass = CharacterTypes[ECharacterType::eMantus];
}

void AACGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (UACGameInstance* GameInstance = Cast<UACGameInstance>(GetGameInstance()))
	{
		SelectedCharacterType = GameInstance->GetCharacterType();
	}

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}


void AACGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NewPlayer->GetPawn()->Destroy();
	const FVector SpawnLocation = FVector(210.f, 80.f, 180.f);
	// const FVector SpawnLocation = FVector(210.f, 80.f, 320.f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer;
	if (UACGameInstance* GameInstance = Cast<UACGameInstance>(GetGameInstance())) 
	{
		if (count == 0) 
		{
			APawn* NewPawn = GetWorld()->SpawnActor<APawn>(CharacterTypes[ECharacterType::eMantus], SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			NewPlayer->Possess(NewPawn);
			++count;
		}
		else
		{
			APawn* NewPawn = GetWorld()->SpawnActor<APawn>(CharacterTypes[ECharacterType::eCollie], SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			NewPlayer->Possess(NewPawn);
		}
		
		if (GameState)
		{
			Cast<AACGameState>(GameState)->AddTargetArray(NewPlayer);
		}
	}
}

void AACGameMode::SpawnMonster(EMonsterType MonsterType)
{
	switch (MonsterType) 
	{
	case EMonsterType::eBoss:
		SpawnBoss(); 
		break;
	case EMonsterType::eMinion:
		SpawnMinion();
		break;
	default: 
		break;
	}
}

void AACGameMode::SpawnBoss()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	AACCharacterBoss* Boss = GetWorld()->SpawnActor< AACCharacterBoss>(BossClass, FVector(1000.f,1000.f, 400.f),FRotator(0.f, 0.f, 0.f),SpawnParams);
}

void AACGameMode::SpawnMinion()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	AACCharacterMinion* Minion = GetWorld()->SpawnActor<AACCharacterMinion>(MinionClass, FVector(1000.f, 1000.f, 400.f), FRotator(0.f, 0.f, 0.f), SpawnParams);
}
