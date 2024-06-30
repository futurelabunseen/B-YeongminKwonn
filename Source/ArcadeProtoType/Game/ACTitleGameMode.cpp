#include "Game/ACTitleGameMode.h"
#include "kismet/GameplayStatics.h"
#include "Player/ACTitlePlayerController.h"

AACTitleGameMode::AACTitleGameMode()
{
	PlayerControllerClass = AACTitlePlayerController::StaticClass();
}

void AACTitleGameMode::CreateServer()
{
	UE_LOG(LogTemp, Log, TEXT("CreateServer"));
	UGameplayStatics::OpenLevel(GetWorld(), FName("ArcadeMap"), true, ((FString)(L"Listen")));
}

void AACTitleGameMode::JoinServer(FString IPAddress)
{
	// "211.34.57.200"
	// "10.99.31.230"
	UE_LOG(LogTemp, Log, TEXT("OpenLevel"));
	// UGameplayStatics::OpenLevel(GetWorld(), FName("10.99.31.230"));
	UGameplayStatics::OpenLevel(GetWorld(), FName(*IPAddress));
}

void AACTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		Iterator->Get()->SetShowMouseCursor(false);
		Iterator->Get()->SetInputMode(FInputModeGameOnly());
	}
}
