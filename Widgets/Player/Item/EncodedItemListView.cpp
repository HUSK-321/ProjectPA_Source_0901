// All copyrights for this code are owned by Aster.


#include "EncodedItemListView.h"
#include "ProjectPA/Widgets/WidgetTypes.h"

void UEncodedItemListView::SetItemList(const TArray<FItemEncodedInfo>& InEncodedItemList)
{
	TArray<UObject*> EncodedItemObjectList;
	for (const FItemEncodedInfo& EncodedItem : InEncodedItemList)
	{
		UItemEncodedInfoObject* NewItemObject = NewObject<UItemEncodedInfoObject>(this);
		NewItemObject->ItemInfo = EncodedItem;
		EncodedItemObjectList.Emplace(NewItemObject);
	}
	this->SetListItems(EncodedItemObjectList);
}
