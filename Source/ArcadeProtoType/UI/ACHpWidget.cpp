
#include "UI/ACHpWidget.h"
#include "Components/Image.h"
#include "Kismet/KismetMaterialLibrary.h"

UACHpWidget::UACHpWidget(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UMaterialInstance> HpMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/UForumHelp_DynRadial/MICs/MI_RadialPercent.MI_RadialPercent'"));
	if (HpMaterialRef.Succeeded())
	{
		HpMaterialInstance = HpMaterialRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> ProfileMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Arcade/UI/Materials/MI_CharacterProfileImage.MI_CharacterProfileImage'"));
	if (ProfileMaterialRef.Succeeded())
	{
		ProfileMaterialInstance = ProfileMaterialRef.Object;
	}
}

void UACHpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	HpRadialImage = Cast<UImage>(GetWidgetFromName(TEXT("RadialHP")));
	CharacterProfileImage = Cast<UImage>(GetWidgetFromName(TEXT("ProfileImage")));
	
}

void UACHpWidget::UpdateHPWidget(float CurrentPercent)
{
	if (HpRadialImage) 
	{
		// UMaterialInstance* MaterialInstance = Cast<UMaterialInstance>(HpRadialImage->GetBrush().GetResourceObject());
		
		// HpMaterialInstanceDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), HpMaterialInstance);

		if (HpMaterialInstanceDynamic != nullptr)
		{
			HpRadialImage->SetBrushFromMaterial(HpMaterialInstanceDynamic);
			HpMaterialInstanceDynamic->SetScalarParameterValue(FName("Mask_Percent"), 1.f-CurrentPercent);
		}
		else
		{
			HpMaterialInstanceDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), HpMaterialInstance);

			HpRadialImage->SetBrushFromMaterial(HpMaterialInstanceDynamic);
			HpMaterialInstanceDynamic->SetScalarParameterValue(FName("Mask_Percent"), 1.f - CurrentPercent);
		}
	}
}

void UACHpWidget::SetImagePath(const TCHAR* ImagePath)
{
	if (ImagePath == nullptr)
		return;

	if (CharacterProfileImage) 
	{
		UMaterialInstanceDynamic* ProfileMaterialDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ProfileMaterialInstance);
		CharacterProfileImage->SetBrushFromMaterial(ProfileMaterialDynamic);
		if (UTexture* ProfileTexture = LoadObject<UTexture>(nullptr, ImagePath)) 
		{
			ProfileMaterialDynamic->SetTextureParameterValue(FName("CharacterImage"), ProfileTexture);
		}
	}
}

void UACHpWidget::SetImageTexture(UTexture* ImageTexture)
{
	if (nullptr == ImageTexture) 
	{
		return;
	}

	if (CharacterProfileImage)
	{
		UMaterialInstanceDynamic* ProfileMaterialDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ProfileMaterialInstance);
		CharacterProfileImage->SetBrushFromMaterial(ProfileMaterialDynamic);
		ProfileMaterialDynamic->SetTextureParameterValue(FName("CharacterImage"), ImageTexture);
	}
}
