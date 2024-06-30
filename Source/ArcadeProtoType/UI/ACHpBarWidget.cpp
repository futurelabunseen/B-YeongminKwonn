
#include "UI/ACHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "ArcadeProtoType.h"

UACHpBarWidget::UACHpBarWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UACHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);
}

void UACHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp);
	}
}
