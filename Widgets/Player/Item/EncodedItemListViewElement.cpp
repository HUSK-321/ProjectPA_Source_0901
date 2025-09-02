// All copyrights for this code are owned by Aster.


#include "EncodedItemListViewElement.h"

#include "EncodedItemListView.h"
#include "PlayerInventoryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProjectPA/Generals/PATypes.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"
#include "ProjectPA/Items/ItemDataTables.h"
#include "ProjectPA/Widgets/WidgetTypes.h"
#include "ProjectPA/Widgets/ViewModel/ItemListViewModel.h"

void UEncodedItemListViewElement::SetElement(const FItemEncodedInfo& InItemInfo)
{
	check(ItemIcon);
	check(ItemName);
	
	IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>();
	check(DataProvider);
	
	if (const FItemDataTable* ItemData = DataProvider->GetItemDataByKey(InItemInfo.GetItemKey()))
	{
		ItemIcon->SetBrushFromSoftTexture(ItemData->ItemIcon);
		ItemName->SetText(ItemData->ItemName);
		ItemKey = InItemInfo.GetItemKey();
	}
}

void UEncodedItemListViewElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (ListItemObject == nullptr)
	{
		RemoveFromParent();
		return;
	}
	
	if (UItemEncodedInfoObject* ItemEncodedInfoObject = Cast<UItemEncodedInfoObject>(ListItemObject))
	{
		SetElement(ItemEncodedInfoObject->ItemInfo);
	}
}

FReply UEncodedItemListViewElement::NativeOnMouseButtonDown(const FGeometry& InGeometry,
                                                            const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (UItemEncodedInfoObject* ListItem = GetListItem<UItemEncodedInfoObject>())
		{
			if (UItemListViewModel* ViewModel = ListItem->ItemListViewModel)
			{
				ViewModel->HandleItemRightClick(ListItem->ItemInfo, IsPlayerSide());
			}
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UEncodedItemListViewElement::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UItemEncodedInfoObject* ListItem = GetListItem<UItemEncodedInfoObject>())
	{
		if (UItemListViewModel* ViewModel = ListItem->ItemListViewModel)
		{
			ViewModel->HandleItemHover(ListItem->ItemInfo, IsPlayerSide());
		}
	}
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UEncodedItemListViewElement::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (UItemEncodedInfoObject* ListItem = GetListItem<UItemEncodedInfoObject>())
	{
		if (UItemListViewModel* ViewModel = ListItem->ItemListViewModel)
		{
			ViewModel->HandleItemUnhover(ListItem->ItemInfo, IsPlayerSide());
		}
	}
	Super::NativeOnMouseLeave(InMouseEvent);
}
