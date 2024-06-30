#include "Game/GamePhase/ACWaitingToStartPhase.h"
#include "Interface/ACGamePhaseInterface.h"
#include "Game/ACGameState.h"
#include "kismet/GameplayStatics.h"
#include "ArcadeProtoType.h"

AACWaitingToStartPhase::AACWaitingToStartPhase()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> BackgroundMusicRef(TEXT("/Script/Engine.SoundWave'/Game/Arcade/Sound/Ch2_BGM_Ininter_bpm100.Ch2_BGM_Ininter_bpm100'"));
	if (BackgroundMusicRef.Object)
	{
		BackgroundMusic = BackgroundMusicRef.Object;
	}
}

void AACWaitingToStartPhase::OnBeginPhase()
{
	// ��������� ����Ѵ�.
	// UGameplayStatics::PlaySound2D(GetWorld(), BackgroundMusic, 1.f);
	// MulticastRPCOnBeginPhase();
	
	
	// Loading Widget�� ����.
}

void AACWaitingToStartPhase::MulticastRPCOnBeginPhase_Implementation()
{
	// UGameplayStatics::PlaySound2D(GetWorld(), BackgroundMusic, 1.f);
}

void AACWaitingToStartPhase::OnPlayPhase(float DeltaTime)
{	

}

void AACWaitingToStartPhase::OnEndPhase()
{
	// Loading Widget�� �ݴ´�.
}
