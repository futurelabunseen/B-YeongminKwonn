
#include "ACLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "ArcadeProtoType.h"
#include "Game/ACGameInstance.h"
#include "Data/ACDataUtility.h"

AACLobbyGameMode::AACLobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void AACLobbyGameMode::CreateServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level01"), true, ((FString)(L"Listen")));
	Cast<UACGameInstance>(GetGameInstance())->SetCharacterType(ECharacterType::eMantus);
}

void AACLobbyGameMode::JoinServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("127.0.0.1"));
	Cast<UACGameInstance>(GetGameInstance())->SetCharacterType(ECharacterType::eCollie);
}

void AACLobbyGameMode::PostLogin(APlayerController* newController)
{
	Super::PostLogin(newController);

	// GameState 는 게임 상태 기반을 유지하며 GameState에는 게임에 참가한 플레이어의 배열이 존재한다.
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2) 
	{
		UWorld* World = GetWorld();
		if (World) 
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Arcade/Maps/ArcadeMap?listen"));
		}
	}
}
