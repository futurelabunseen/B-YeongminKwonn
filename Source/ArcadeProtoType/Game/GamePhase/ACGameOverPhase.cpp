// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GamePhase/ACGameOverPhase.h"

AACGameOverPhase::AACGameOverPhase()
{
}

void AACGameOverPhase::OnBeginPhase()
{
	// ���� �̹����� ����.
}

void AACGameOverPhase::OnPlayPhase(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("AACGameOverPhase::OnPlayPhase"));
}

void AACGameOverPhase::OnEndPhase()
{
}
