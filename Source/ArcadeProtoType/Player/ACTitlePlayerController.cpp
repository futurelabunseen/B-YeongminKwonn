#include "Player/ACTitlePlayerController.h"
#include "kismet/GameplayStatics.h"

AACTitlePlayerController::AACTitlePlayerController()
{
}

void AACTitlePlayerController::CreateServer()
{
	UE_LOG(LogTemp, Log, TEXT("CreateServer"));
	UGameplayStatics::OpenLevel(GetWorld(), FName("ArcadeMap"), true, ((FString)(L"Listen")));
}

void AACTitlePlayerController::JoinServer()
{
// "211.34.57.200"
//	"10.99.31.230"
 // Level 이름 대신 IP 주소.

	UE_LOG(LogTemp, Log, TEXT("OpenLevel"));
	UGameplayStatics::OpenLevel(GetWorld(), FName("10.99.31.230"));
}

void AACTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
}
