// All copyrights for this code are owned by Aster.

#include "ItemListViewModel.h"

#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Components/InteractionComponent.h"
#include "ProjectPA/Components/InventoryComponent.h"
#include "ProjectPA/Items/BagItemBase.h"
#include "ProjectPA/Widgets/WidgetTypes.h"

UItemListViewModel::UItemListViewModel()
	: OwnerUID(INDEX_NONE)
	, PlayerInventoryWeight(0)
	, CurPlayerBagIndex(0)
	, CurNearbyListIndex(0)
{
	DefaultModelName = TEXT("ItemList");
}

void UItemListViewModel::Initialize(UObject* OwningObject)
{
	Super::Initialize(OwningObject);

	if (AActor* OwningActor = Cast<AActor>(OwningObject))
	{
		InteractionComponent = OwningActor->FindComponentByClass<UInteractionComponent>();
		InventoryComponent = OwningActor->FindComponentByClass<UInventoryComponent>();

		if (InteractionComponent)
		{
			InteractionComponent->OnNearbyItemListChanged.AddUObject(this, &UItemListViewModel::OnNearbyItemListChanged);
			OnNearbyItemListChanged();
		}

		if (InventoryComponent)
		{
			InventoryComponent->OnPlayerItemListChanged.AddUObject(this, &UItemListViewModel::OnPlayerItemListChanged);
			InventoryComponent->OnWeightChanged.AddUObject(this, &UItemListViewModel::OnWeightChanged);

			InventoryComponent->CalculateWeight();
			OnPlayerItemListChanged();
		}

		if (IPAActor* PAActor = Cast<IPAActor>(OwningActor))
		{
			OwnerUID = PAActor->GetUID();
		}
	}
}

FText UItemListViewModel::GetPlayerInventoryWeightText() const
{
	return FText::AsNumber(PlayerInventoryWeight);
}

void UItemListViewModel::SetPlayerBags(const TArray<UBagItemBase*>& NewPlayerBags)
{
	PlayerBags = NewPlayerBags;
	UpdateDisplayedPlayerItems();

	TArray<UTexture2D*> ItemTextures;
	for (int i = 0; i < PlayerBags.Num(); ++i)
	{
		ItemTextures.Emplace(PlayerBags[i] ? PlayerBags[i]->GetItemIcon() : nullptr);
	}
	OnPlayerBagCountChanged.Broadcast(ItemTextures);
}

void UItemListViewModel::SetNearbyContainers(const TArray<TScriptInterface<IItemContainer>>& NewNearbyContainers)
{
	NearbyContainers = NewNearbyContainers;
	UpdateDisplayedNearbyContainers();

	TArray<UTexture2D*> ItemTextures;
	for (int i = 0; i < NearbyContainers.Num(); ++i)
	{
		ItemTextures.Emplace(NearbyContainers[i] ? NearbyContainers[i]->GetContainerIcon() : nullptr);
	}
	OnNearbyListCountChanged.Broadcast(ItemTextures);
}

void UItemListViewModel::RequestUseContextItem()
{
	if (InventoryComponent)
	{
		const int32 ContextItemOwnerUID = GetContextItemOwnerUID();
		InventoryComponent->UseItem(ContextMenuItemInfo, ContextItemOwnerUID);
	}
}

void UItemListViewModel::RequestRemoveContextItem()
{
	if (CurContextIsPlayerSide == false || IsValid(InventoryComponent) == false)
	{
		return;
	}

	InventoryComponent->RemoveItem(CurPlayerBagIndex, ContextMenuItemInfo);
}

void UItemListViewModel::RequestAcquireContextItem()
{
	if (InventoryComponent && CurContextIsPlayerSide == false)
	{
		if (NearbyContainers.IsValidIndex(CurNearbyListIndex))
		{
			const int32 ItemOwnerUID = NearbyContainers[CurNearbyListIndex]->GetContainerUID();
			InventoryComponent->TryGetItem(ContextMenuItemInfo, ItemOwnerUID);
		}
	}
}

void UItemListViewModel::HandleItemRightClick(const FItemEncodedInfo& ItemInfo, bool bIsPlayerSide)
{
	// Clear tooltip when contextmenu valid
	HoveredItemInfo = FItemEncodedInfo();
	
	ContextMenuItemInfo = ItemInfo;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ContextMenuItemInfo);

	CurContextIsPlayerSide = bIsPlayerSide;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CurContextIsPlayerSide);
}

void UItemListViewModel::HandleItemHover(const FItemEncodedInfo& ItemInfo, bool bIsPlayerSide)
{
	HoveredItemInfo = ItemInfo;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HoveredItemInfo);
}

void UItemListViewModel::HandleItemUnhover(const FItemEncodedInfo& ItemInfo, bool bIsPlayerSide)
{
	if (HoveredItemInfo == ItemInfo)
	{
		HoveredItemInfo = FItemEncodedInfo();
	}
}

void UItemListViewModel::ClearContextMenu()
{
	ContextMenuItemInfo = FItemEncodedInfo();
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ContextMenuItemInfo);
}

void UItemListViewModel::UpdateDisplayedPlayerItems()
{
	TArray<UObject*> NewDisplayedItems;

	if (PlayerBags.IsValidIndex(CurPlayerBagIndex) && IsValid(PlayerBags[CurPlayerBagIndex]))
	{
		TArray<FItemEncodedInfo> ItemInfos;
		PlayerBags[CurPlayerBagIndex]->GetItemList(ItemInfos);

		for (const FItemEncodedInfo& ItemInfo : ItemInfos)
		{
			NewDisplayedItems.Add(MakeItemEncodedObject(ItemInfo));
		}
	}

	UE_MVVM_SET_PROPERTY_VALUE(DisplayedPlayerItems, NewDisplayedItems);
}

UItemEncodedInfoObject* UItemListViewModel::MakeItemEncodedObject(const FItemEncodedInfo& ItemInfo)
{
	UItemEncodedInfoObject* NewItemObject = NewObject<UItemEncodedInfoObject>(this);
	if (NewItemObject)
	{
		NewItemObject->ItemInfo = ItemInfo;
		NewItemObject->ItemListViewModel = this;
	}
	return NewItemObject;
}

void UItemListViewModel::UpdateDisplayedNearbyContainers()
{
	TArray<UObject*> NewDisplayedItems;

	if (NearbyContainers.IsValidIndex(CurNearbyListIndex) && NearbyContainers[CurNearbyListIndex].GetInterface() != nullptr)
	{
		TArray<FItemEncodedInfo> ItemInfos;
		NearbyContainers[CurNearbyListIndex]->GetItemList(ItemInfos);

		for (const FItemEncodedInfo& ItemInfo : ItemInfos)
		{
			NewDisplayedItems.Add(MakeItemEncodedObject(ItemInfo));
		}
	}

	DisplayedNearbyItems = NewDisplayedItems;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(DisplayedNearbyItems);
}

void UItemListViewModel::OnPlayerItemListChanged()
{
	if (InventoryComponent)
	{
		SetPlayerBags(InventoryComponent->GetPlayerBags());
	}
}

void UItemListViewModel::OnNearbyItemListChanged()
{
	if (InteractionComponent)
	{
		SetNearbyContainers(InteractionComponent->GetNearbyItemList());
	}
}

void UItemListViewModel::OnWeightChanged(const float NewWeight)
{
	SetPlayerInventoryWeight(NewWeight);

	PlayerInventoryWeightText = FText::AsNumber(NewWeight);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(PlayerInventoryWeightText);
}

int32 UItemListViewModel::GetContextItemOwnerUID() const
{
	if (CurContextIsPlayerSide)
	{
		return OwnerUID;
	}

	return NearbyContainers.IsValidIndex(CurNearbyListIndex) ? NearbyContainers[CurNearbyListIndex]->GetContainerUID() : INDEX_NONE;
}

