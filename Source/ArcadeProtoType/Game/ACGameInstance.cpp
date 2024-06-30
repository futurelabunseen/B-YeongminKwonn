
#include "Game/ACGameInstance.h"
#include "ArcadeProtoType.h"

UACGameInstance::UACGameInstance() 
{
	DataManager = CreateDefaultSubobject<UACDataManager>(TEXT("DataManager"));
}

void UACGameInstance::Init()
{
	Super::Init();
}

void UACGameInstance::SetCharacterType(ECharacterType InCharacterType)
{
	CurrentCharacterType = InCharacterType;
	if (CurrentCharacterType == ECharacterType::eMantus) 
	{
		UE_LOG(LogTemp, Log, TEXT("TEST:: eMantus"));
	}
	else 
	{
		UE_LOG(LogTemp, Log, TEXT("TEST:: eCollie"));
	}
}

ECharacterType UACGameInstance::GetCharacterType() const
{
	return CurrentCharacterType;
}
