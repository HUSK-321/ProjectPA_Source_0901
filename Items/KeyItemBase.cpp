// All copyrights for this code are owned by Aster.


#include "KeyItemBase.h"
#include "ItemDataTables.h"

void UKeyItemBase::InitializeItemInfo(const FItemDataTable* DataTableInfo)
{
	Super::InitializeItemInfo(DataTableInfo);
	if (const FKeyItemDataTable* ConsumeItemInfo = static_cast<const FKeyItemDataTable*>(DataTableInfo))
	{
		KeyCode = ConsumeItemInfo->KeyCode;
	}

	bIsUsable = false;
}
