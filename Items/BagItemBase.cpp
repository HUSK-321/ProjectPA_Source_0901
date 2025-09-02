#include "BagItemBase.h"
#include "ItemDataTables.h"
#include "ProjectPA/Generals/PATypes.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"

UBagItemBase::UBagItemBase()
	: MaxWeight(10.f)
	, CurrentWeight(0.0f)
{
	ItemType = EItemType::Bag;
	CannotContainTypes = {EItemType::Bag};
}

void UBagItemBase::InitializeItemInfo(const FItemDataTable* DataTableInfo)
{
	Super::InitializeItemInfo(DataTableInfo);
	if (const FBagItemDataTable* BagItemInfo = static_cast<const FBagItemDataTable*>(DataTableInfo))
	{
		MaxWeight = BagItemInfo->MaxWeight;
		CannotContainTypes = BagItemInfo->CannotContainTypes;
	}
}

EGetItemError UBagItemBase::CanGetItem(const FItemEncodedInfo& InItem)
{
	IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>();
	if (DataProvider == nullptr)
	{
		return EGetItemError::GeneralError;
	}
	
	const FItemDataTable* ItemData = DataProvider->GetItemDataByKey(InItem.GetItemKey());
	if (ItemData == nullptr)
	{
		return EGetItemError::GeneralError;
	}
	if (CurrentWeight + ItemData->Weight > MaxWeight)
	{
		return EGetItemError::OutOfWeight;
	}
	if (CannotContainTypes.Contains(ItemData->ItemType))
	{
		return EGetItemError::NotCompatibility;
	}
	return EGetItemError::NoError;
}

EGetItemError UBagItemBase::TryAddItem(const FItemEncodedInfo& InItem)
{
	EGetItemError Result = CanGetItem(InItem);
	if (Result != EGetItemError::NoError)
	{
		return Result;
	}

	IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>();
	check(DataProvider);

	if (DataProvider != nullptr)
	{
		if (const FItemDataTable* ItemData = DataProvider->GetItemDataByKey(InItem.GetItemKey()))
		{
			ItemList.Emplace(InItem);
			CurrentWeight += ItemData->Weight;
			return EGetItemError::NoError;	
		}
	}

	return EGetItemError::GeneralError;
}

bool UBagItemBase::TryToRemoveItem(const FItemEncodedInfo& InItem)
{
	for (auto It = ItemList.CreateIterator(); It; ++It)
	{
		FItemEncodedInfo& Item = *It;
		if (Item == InItem)
		{
			It.RemoveCurrent();
			IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>();
			check(DataProvider);

			if (DataProvider != nullptr)
			{
				if (const FItemDataTable* ItemData = DataProvider->GetItemDataByKey(InItem.GetItemKey()))
				{
					CurrentWeight -= ItemData->Weight;
				}
			}
			return true;
		}
	}
	return false;
}

bool UBagItemBase::HaveItem(const FName& ItemKeyToFind)
{
	for (const FItemEncodedInfo& ItemInfo : ItemList)
	{
		if (ItemInfo.GetItemKey() == ItemKeyToFind)
		{
			return true;
		}
	}
	return false;
}

void UBagItemBase::GetItemList(TArray<FItemEncodedInfo>& OutItemList) const
{
	OutItemList.Append(ItemList);
}

FItemEncodedInfo* UBagItemBase::GetItem(const FItemEncodedInfo& InItemEncodedInfo)
{
	for (FItemEncodedInfo& Item : ItemList)
	{
		if (Item == InItemEncodedInfo)
		{
			return &Item;
		}
	}

	return nullptr;
}
