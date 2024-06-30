#include "UI/ACAmmoWidget.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"


UACAmmoWidget::UACAmmoWidget(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UMaterialInstance> AmmoMaterialRef
		(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/UForumHelp_DynRadial/MICs/MI_RadialPercent.MI_RadialPercent'"));
	if (AmmoMaterialRef.Succeeded())
	{
		AmmoMaterialInstance = AmmoMaterialRef.Object;
	}
}

void UACAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AmmoRadialImage = Cast<UImage>(GetWidgetFromName(TEXT("RadialAmmo")));
}

void UACAmmoWidget::UpdateHPWidget(float CurrentPercent)
{
	if (AmmoRadialImage)
	{
		if (AmmoMaterialInstanceDynamic != nullptr)
		{
			AmmoRadialImage->SetBrushFromMaterial(AmmoMaterialInstanceDynamic);
			AmmoMaterialInstanceDynamic->SetScalarParameterValue(FName("Mask_Percent"), 1.f - CurrentPercent);
		}
		else
		{
			AmmoMaterialInstanceDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), AmmoMaterialInstance);

			AmmoRadialImage->SetBrushFromMaterial(AmmoMaterialInstanceDynamic);
			AmmoMaterialInstanceDynamic->SetScalarParameterValue(FName("Mask_Percent"), 1.f - CurrentPercent);
		}
	}

}
