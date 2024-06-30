#include "UI/ACTitleGame.h"
#include "kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Player/ACPlayerController.h"
#include "Player/ACTitlePlayerController.h"
#include "Game/ACTitleGameMode.h"

void UACTitleGame::CreateServer()
{
	//UGameplayStatics::OpenLevel(GetWorld(), FName("ArcadeMap"), true, ((FString)(L"Listen")));
	// if (AACTitlePlayerController* ArcadePlayerController = Cast< AACTitlePlayerController>(GetOwningPlayer()))
	// {
	// 	ArcadePlayerController -> CreateServer();
	// 	UE_LOG(LogTemp, Log, TEXT("CreateServer PlayerCOntroller"));
	// }

	if(AACTitleGameMode* TitleGS = GetWorld()->GetAuthGameMode<AACTitleGameMode>() )
	{
		TitleGS->CreateServer();
		UE_LOG(LogTemp, Log, TEXT("CreateServer "));
	}
}

void UACTitleGame::JoinServer()
{
	// "211.34.57.200"
	//	"10.99.31.230"
	 // Level 이름 대신 IP 주소.
	//UGameplayStatics::OpenLevel(GetWorld(), FName("211.34.57.200"));

	// if (AACTitlePlayerController* ArcadePlayerController = Cast< AACTitlePlayerController>(GetOwningPlayer()))
	// {
	// 	ArcadePlayerController->JoinServer();
	// 	UE_LOG(LogTemp, Log, TEXT("JoinServer PlayerCOntroller"));
	// }
	// UE_LOG(LogTemp, Log, TEXT("JoinServer"));

	if (AACTitleGameMode* TitleGS = GetWorld()->GetAuthGameMode<AACTitleGameMode>())
	{
		FString IPText = IPTextBox->GetText().ToString();
		TitleGS->JoinServer(IPText);
		UE_LOG(LogTemp, Log, TEXT("JoinServer"));
	}
}

UACTitleGame::UACTitleGame(const FObjectInitializer& ObjectInitializer)
{
}

void UACTitleGame::NativeConstruct()
{
	Super::NativeConstruct();

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UACTitleGame::CreateServer);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UACTitleGame::JoinServer);
	}
}
