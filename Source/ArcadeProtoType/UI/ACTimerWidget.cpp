#include "UI/ACTimerWidget.h"
#include "Components/TextBlock.h"

UACTimerWidget::UACTimerWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UACTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UACTimerWidget::UpdateTimerTextWidget(FString InCurrentLeftTime)
{
	CurrentLeftTime->SetText(FText::FromString(InCurrentLeftTime));
}
