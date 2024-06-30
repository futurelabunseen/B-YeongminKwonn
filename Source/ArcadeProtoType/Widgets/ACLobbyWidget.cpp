#include "Widgets/ACLobbyWidget.h"
// #include "Components/Button.h"
// #include "Components/TextBlock.h"
#include "Game/ACLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Data/ACDataUtility.h"
#include "Game/ACGameInstance.h"

// 만들어야 할 UI 컴포넌트
// 캐릭터를 선택하는 버튼 2개 : 캐릭터 1, 캐릭터 2
// Create Game / Join Game : 호스트인지 조인인지 버튼 두 개 만들기

// 캐릭터 선택 유도하기
// 선택한 캐릭터를 저장하거나 , 선택한 순간 바로 GameMode나 GameInstance로 보내야 한다.
// 

void UACLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UACLobbyWidget::CreateServerHandler()
{
	if (AACLobbyGameMode* LobbyGameMode = Cast<AACLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		LobbyGameMode->CreateServer();
		SelectCharacterMantus();
	}
	
	// SelectCharacterMantus();
}

void UACLobbyWidget::JoinServerHandler()
{
	if (AACLobbyGameMode* LobbyGameMode = Cast<AACLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		LobbyGameMode->JoinServer();
		SelectCharacterCollie();
	}
}

void UACLobbyWidget::SelectCharacterMantus()
{
	if (UACGameInstance* GameInstance = Cast<UACGameInstance>(GetGameInstance())) 
	{
		GameInstance->SetCharacterType(ECharacterType::eMantus);
	}
}

void UACLobbyWidget::SelectCharacterCollie()
{
	if (UACGameInstance* GameInstance = Cast<UACGameInstance>(GetGameInstance()))
	{
		GameInstance->SetCharacterType(ECharacterType::eCollie);
	}
}
