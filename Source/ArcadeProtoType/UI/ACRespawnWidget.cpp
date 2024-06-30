#include "UI/ACRespawnWidget.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"

UACRespawnWidget::UACRespawnWidget(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UMaterialInstance> RespawnMaterialRef
	(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/UForumHelp_DynRadial/MICs/MI_RadialPercent.MI_RadialPercent'"));
	
	if (RespawnMaterialRef.Succeeded())
	{
		RespawnMaterialInstance = RespawnMaterialRef.Object;
	}
}

void UACRespawnWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UACRespawnWidget::UpdateWidget(float Value)
{
	if (RespawnRadialImage)
	{
		if (RespawnMaterialInstanceDynamic != nullptr)
		{
			RespawnRadialImage->SetBrushFromMaterial(RespawnMaterialInstanceDynamic);
			RespawnMaterialInstanceDynamic->SetScalarParameterValue(FName("Mask_Percent"), 1.f- Value);
		}
		else
		{
			RespawnMaterialInstanceDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), RespawnMaterialInstance);

			RespawnRadialImage->SetBrushFromMaterial(RespawnMaterialInstanceDynamic);
			RespawnMaterialInstanceDynamic->SetScalarParameterValue(FName("Mask_Percent"), 1.f - Value);
		}
	}
}
