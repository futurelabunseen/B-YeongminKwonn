#include "UI/ACAimAssistWidget.h"
#include "Components/SizeBox.h"

UACAimAssistWidget::UACAimAssistWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UACAimAssistWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RightBox = Cast<USizeBox>(GetWidgetFromName(TEXT("Crosshair_RightBox")));
	LeftBox = Cast<USizeBox>(GetWidgetFromName(TEXT("Crosshair_LeftBox")));
	TopBox = Cast<USizeBox>(GetWidgetFromName(TEXT("Crosshair_TopBox")));
	BottomBox = Cast<USizeBox>(GetWidgetFromName(TEXT("Crosshair_BottomBox")));

	SizeBoxArray.Add(RightBox);
	SizeBoxArray.Add(LeftBox);
	SizeBoxArray.Add(TopBox);
	SizeBoxArray.Add(BottomBox);

	// Right
	DefaultTranslationArray.Add(FVector2D(25.0f, 0.0f));
	// Left
	DefaultTranslationArray.Add(FVector2D(-25.0f, 0.0f));
	// Top
	DefaultTranslationArray.Add(FVector2D(0.0f, -25.0f));
	//Bottom
	DefaultTranslationArray.Add(FVector2D(0.0f, 25.0f));
}

void UACAimAssistWidget::UpdateAimAssistWidget(bool bIsAiming)
{
	if (DefaultTranslationArray.Num() <= 0)
	{
		return;
	}

	if (bIsAiming) 
	{
		for (USizeBox* SizeBox : SizeBoxArray) 
		{
			SizeBox->SetRenderTranslation(FVector2D(0.f, 0.f));
		}
	}
	else 
	{
		for (int i=0; i< 4; ++i)
		{
			if (SizeBoxArray[i]) 
			{
				if (SizeBoxArray[i])
				{
				SizeBoxArray[i]->SetRenderTranslation(DefaultTranslationArray[i]);
				}
			}
		}
	}
}
