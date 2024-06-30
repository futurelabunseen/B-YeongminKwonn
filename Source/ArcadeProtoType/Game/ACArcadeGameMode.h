#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Data/ACDataUtility.h"
#include "Interface/ACAISpawner.h"
#include "ACArcadeGameMode.generated.h"


/**
 * 
 */
UCLASS()
class ARCADEPROTOTYPE_API AACArcadeGameMode : public AGameMode// , public IACGamePhaseInterface
{
	GENERATED_BODY()
public:
	AACArcadeGameMode();
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	void PlayerLeftGame(class AACPlayerState* ArcadePlayerState);
private:
	TArray<UClass*> CharacterTypes;
	ECharacterType SelectedCharacterType;

	int32 PlayerCount = 0;
};
