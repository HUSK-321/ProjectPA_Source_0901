// All copyrights for this code are owned by Aster.


#include "ConsumeItemStatWidget.h"
#include "ProjectPA/Generals/PATypes.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "ProjectPA/Characters/CharacterAttributes/PlayerAttributeSet.h"
#include "ProjectPA/Items/ItemDataTables.h"
#include "Styling/SlateColor.h"

void UConsumeItemStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CacheStatTexts();
}

void UConsumeItemStatWidget::SetItemStatus(const FItemDataTable* InItemData, const FItemEncodedInfo& InItemInfo)
{
	Super::SetItemStatus(InItemData, InItemInfo);

	const FConsumeItemDataTable* ConsumeItem = static_cast<const FConsumeItemDataTable*>(InItemData);
	if (ConsumeItem)
	{
		if (ExpireDate)
		{
			const FGameTimeInfo ItemManufacturingDate = InItemInfo.GetItemMadeDate();
			const FGameTimeInfo ItemExpireDate = ConsumeItem->ExpireDate;

			const FGameTimeInfo FinalDate = ItemManufacturingDate + ItemExpireDate;
			ExpireDate->SetText(FText::FromString(FinalDate.ToString()));
		}

		for (auto& StatText : StatTexts)
		{
			const FGameplayAttribute& Attribute = StatText.Key;
			if (UTextBlock* TextBlock = StatText.Value)
			{
				const float StatEffect =  ConsumeItem->GetConsumeEffect(Attribute);
				FString StatEffectString = FString::Printf(TEXT("%.1f"), StatEffect);
				TextBlock->SetText(FText::FromString(StatEffectString));

				FLinearColor PositiveColor = FLinearColor(0.2f, 1.0f, 0.0f);
				FLinearColor NegativeColor = FLinearColor(1.0f, 0.2f, 0.0f);

				if (Attribute != UPlayerAttributeSet::GetHealthAttribute() && Attribute != UPlayerAttributeSet::GetStaminaAttribute())
				{
					Swap(PositiveColor, NegativeColor);
				}
				const FSlateColor TextColor = StatEffect > 0.f
					                              ? PositiveColor
					                              : (StatEffect < 0.f
						                                 ? NegativeColor
						                                 : FLinearColor(1.0f, 1.0f, 1.0f));
				TextBlock->SetColorAndOpacity(TextColor);
			}
		}
	}
}

void UConsumeItemStatWidget::CacheStatTexts()
{
	if (WidgetTree == nullptr || StatTexts.Num() > 0)
	{
		return;
	}

	if (UTextBlock* FoundWidget = Cast<UTextBlock>(WidgetTree->FindWidget(TEXT("HealthTextBlock"))))
	{
		StatTexts.Emplace(UPlayerAttributeSet::GetHealthAttribute(), FoundWidget);
	}
	if (UTextBlock* FoundWidget = Cast<UTextBlock>(WidgetTree->FindWidget(TEXT("StaminaTextBlock"))))
	{
		StatTexts.Emplace(UPlayerAttributeSet::GetStaminaAttribute(), FoundWidget);
	}
	if (UTextBlock* FoundWidget = Cast<UTextBlock>(WidgetTree->FindWidget(TEXT("HungerTextBlock"))))
	{
		StatTexts.Emplace(UPlayerAttributeSet::GetHungerAttribute(), FoundWidget);
	}
	if (UTextBlock* FoundWidget = Cast<UTextBlock>(WidgetTree->FindWidget(TEXT("ThirstyTextBlock"))))
	{
		StatTexts.Emplace(UPlayerAttributeSet::GetThirstyAttribute(), FoundWidget);
	}
	if (UTextBlock* FoundWidget = Cast<UTextBlock>(WidgetTree->FindWidget(TEXT("StressTextBlock"))))
	{
		StatTexts.Emplace(UPlayerAttributeSet::GetStressAttribute(), FoundWidget);
	}
	if (UTextBlock* FoundWidget = Cast<UTextBlock>(WidgetTree->FindWidget(TEXT("TiredTextBlock"))))
	{
		StatTexts.Emplace(UPlayerAttributeSet::GetTiredAttribute(), FoundWidget);
	}
	if (UTextBlock* FoundWidget = Cast<UTextBlock>(WidgetTree->FindWidget(TEXT("SleepTextBlock"))))
	{
		StatTexts.Emplace(UPlayerAttributeSet::GetSleepAttribute(), FoundWidget);
	}
}
