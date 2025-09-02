// All copyrights for this code are owned by Aster.
#include "PAGameInstance.h"

#include "ProjectPA/PALog.h"
#include "ProjectPA/Generals/PAUtils.h"
#include "ProjectPA/Items/BagItemBase.h"
#include "ProjectPA/Items/ItemBase.h"
#include "ProjectPA/Items/ItemDataTables.h"
#include "ProjectPA/Items/KeyItemBase.h"

void UPAGameInstance::Init()
{
	Super::Init();
	ItemDataTableMap.Add(TEXT("Item"), {nullptr, UPAUtils::LoadDataTable(TEXT("/Game/02_Datas/Items/DT_DefaultItem.DT_DefaultItem"))});
	ItemDataTableMap.Add(TEXT("Weapon"), {nullptr, UPAUtils::LoadDataTable(TEXT("/Game/02_Datas/Items/DT_WeaponItem.DT_WeaponItem"))});
	ItemDataTableMap.Add(TEXT("Consume"), {nullptr, UPAUtils::LoadDataTable(TEXT("/Game/02_Datas/Items/DT_ConsumeItem.DT_ConsumeItem"))});
	ItemDataTableMap.Add(TEXT("Bag"), {UBagItemBase::StaticClass(), UPAUtils::LoadDataTable(TEXT("/Game/02_Datas/Items/DT_BagItem.DT_BagItem"))});
	ItemDataTableMap.Add(TEXT("Key"), {nullptr, UPAUtils::LoadDataTable(TEXT("/Game/02_Datas/Items/DT_KeyItem.DT_KeyItem"))});
	ItemDataTableMap.Add(TEXT("Wearing"), {nullptr, UPAUtils::LoadDataTable(TEXT("/Game/02_Datas/Items/DT_WearingItem.DT_WearingItem"))});
}

FItemDataTable* UPAGameInstance::GetItemDataByKey(const FName& ItemTableKey) const
{
	const FItemClassAndDataTable* ItemClassAndDataTable = GetItemClassAndDataTable(ItemTableKey);
	if (ItemClassAndDataTable == nullptr || ItemClassAndDataTable->DataTable == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetItemDataByKey Failed : %s"), *ItemTableKey.ToString());
		return nullptr;
	}
	return ItemClassAndDataTable->DataTable->FindRow<FItemDataTable>(ItemTableKey, TEXT("GetItemDataByKey"));
}

UItemBase* UPAGameInstance::GetItemByKey(const FName& ItemTableKey, UObject* ItemOuter, const FItemEncodedInfo& ItemEncodedInfo)
{
	const FItemClassAndDataTable* ItemClassAndDataTable = GetItemClassAndDataTable(ItemTableKey);
	if (ItemClassAndDataTable == nullptr || ItemClassAndDataTable->DataTable == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetItemByKey Failed : %s"), *ItemTableKey.ToString());
		return nullptr;
	}

	UObject* Outer = (ItemOuter == nullptr) ? GetWorld() : ItemOuter;
	UItemBase* NewItem = NewObject<UItemBase>(Outer, ItemClassAndDataTable->ItemClass);
	if (NewItem)
	{
		NewItem->SetItemEncodedInfo(ItemEncodedInfo);
		NewItem->InitializeItemInfo(ItemClassAndDataTable->DataTable->FindRow<FItemDataTable>(ItemTableKey, TEXT("GetItemDataByKey")));
	}
	return NewItem;
}

const FItemClassAndDataTable* UPAGameInstance::GetItemClassAndDataTable(const FName& ItemTableKey) const
{
	if (ItemTableKey.IsNone())
	{
		PALog_E(TEXT("ItemTableKey is empty."));
		return nullptr;
	}

	// Check prefix of ItemTableKey
	const FString KeyString = ItemTableKey.ToString();
	FString Prefix;
	FString RowName;
	if (!KeyString.Split(TEXT("_"), &Prefix, &RowName))
	{
		PALog_E(TEXT("Invalid ItemTableKey format: %s"), *KeyString);
		return nullptr;
	}

	return ItemDataTableMap.Find(Prefix);
}

