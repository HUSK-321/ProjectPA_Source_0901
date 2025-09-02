#include "ItemBase.h"
#include "ItemDataTables.h"
#include "Net/UnrealNetwork.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"

UItemBase::UItemBase()
	: Weight(0.1f)
	, ItemRemainPercentage(1.f)
	, bIsUsable(true)
	, bDataInitialized(false)
{
}

void UItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemBase, ItemRemainPercentage);
	DOREPLIFETIME(UItemBase, ItemEncodedInfo);
}

void UItemBase::InitializeItemInfo(const FItemDataTable* DataTableInfo)
{
	// TODO : 해당 데이터에 대한 포인터 타입으로 변경?
	if (!bDataInitialized && DataTableInfo)
	{
		ItemType = DataTableInfo->ItemType;
		ItemIcon = DataTableInfo->ItemIcon;
		ItemName = DataTableInfo->ItemName;
		ItemDescription = DataTableInfo->ItemDescription;
		Weight = DataTableInfo->Weight;
	}
	OnItemInfoSet.Broadcast();
}

void UItemBase::EquipItem(AGameCharacterBase* InUser)
{
	if (IsEquipped() || InUser == nullptr)
	{
		return;
	}

	if (InUser)
	{
		SetOwner(InUser);
	}
	
	bEquip = true;
}

void UItemBase::WearItem(AGameCharacterBase* InUser)
{
	if(bWear)
	{
		return;
	}

	bWear = true;
	if (InUser)
	{
		SetOwner(InUser);
	}
}

void UItemBase::UnEquipItem(AGameCharacterBase* InUser)
{
	if(bEquip == false)
	{
		return;
	}
	
	bEquip = false;
	if (InUser)
	{
		SetOwner(InUser);
		//InUser->UnEquipItem(this);
	}
}

void UItemBase::UnWearItem(AGameCharacterBase* InUser)
{
	if (bWear == false)
	{
		return;
	}
	
	bWear = false;
}

void UItemBase::SetItemEncodedInfo(const FItemEncodedInfo& InEncodedInfo)
{
	ItemEncodedInfo = InEncodedInfo;
}

bool UItemBase::IsSameItem(const FItemEncodedInfo& Compare) const
{
	return ItemEncodedInfo == Compare;
}

void UItemBase::SetOwner(AGameCharacterBase* InOwner)
{
	if (InOwner == nullptr)
	{
		LooseOwner();
	}
	Owner = InOwner;
}

void UItemBase::LooseOwner()
{
	Owner = nullptr;
}

bool UItemBase::IsBelongedToLocalPlayer() const
{
	return (Owner != nullptr && Owner->IsLocallyControlled());
}

void UItemBase::DestroyItem()
{
	// @TODO_멀티에서 남이 이걸 지웠을때를 생각해라...
	LooseOwner();

	// @TODO_husk321 해당 부분을 조금 늦게
	//BeginDestroy();
}

void UItemBase::OnRep_ItemEncodedInfo()
{
	if (GetWorld() == nullptr)
	{
		return;
	}
	
	IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>();
	check(DataProvider);

	if (DataProvider != nullptr)
	{
		if (const FItemDataTable* ItemData = DataProvider->GetItemDataByKey(ItemEncodedInfo.GetItemKey()))
		{
			InitializeItemInfo(ItemData);
		}
	}
}
