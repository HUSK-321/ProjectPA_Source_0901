// All copyrights for this code are owned by Aster.

#include "ItemTooltipWidget.h"
#include "ItemStatWidgetBase.h"
#include "Blueprint/WidgetTree.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "ProjectPA/Generals/PATypes.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"
#include "ProjectPA/Items/ItemDataTables.h"

void UItemTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CacheStatusWidgets();
}

void UItemTooltipWidget::SetTooltipWidget(const FItemEncodedInfo& ItemInfo)
{
	IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>();
	check(DataProvider);
	if (const FItemDataTable* ItemData = DataProvider->GetItemDataByKey(ItemInfo.GetItemKey()))
	{
		if (ItemName && ItemDescription && WeightText)
		{
			ItemName->SetText(ItemData->ItemName);
			ItemDescription->SetText(ItemData->ItemDescription);
			WeightText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ItemData->Weight)));
		}

		if (ItemToolTipBox != nullptr)
		{
			ItemToolTipBox->ClearChildren();
		}
	
		if (StatusWidgets.Find(CurTooltipItemType))
		{
			StatusWidgets[CurTooltipItemType]->SetVisibility(ESlateVisibility::Hidden);
			ItemToolTipBox->RemoveChild(StatusWidgets[CurTooltipItemType]);
		}
	
		const EItemType ItemType = ItemData->ItemType;
		if (StatusWidgets.Find(ItemType) && StatusWidgets[ItemType] != nullptr)
		{
			StatusWidgets[ItemType]->SetItemStatus(ItemData, ItemInfo);
			StatusWidgets[ItemType]->SetVisibility(ESlateVisibility::HitTestInvisible);
			ItemToolTipBox->AddChildToVerticalBox(StatusWidgets[ItemType]);
			CurTooltipItemType = ItemType;
		}
	}
}

void UItemTooltipWidget::CacheStatusWidgets()
{
	if (WidgetTree == nullptr || StatusWidgets.Num() > 0)
	{
		return;
	}

	if (UItemStatWidgetBase* FoundWidget = Cast<UItemStatWidgetBase>(WidgetTree->FindWidget(TEXT("ConsumeItemStat"))))
	{
		StatusWidgets.Emplace(EItemType::Consumable, FoundWidget);
	}
	if (UItemStatWidgetBase* FoundWidget = Cast<UItemStatWidgetBase>(WidgetTree->FindWidget(TEXT("WeaponItemStat"))))
	{
		StatusWidgets.Emplace(EItemType::Weapon, FoundWidget);
	}
	
}
