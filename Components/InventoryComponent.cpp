// All copyrights for this code are owned by Aster.


#include "InventoryComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "ProjectPA/Abilities/AbilityObjects/AbilityItemPayloads.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Interfaces/ItemContainer.h"
#include "ProjectPA/Items/BagItemBase.h"
#include "ProjectPA/Network/UIDContainer.h"

UInventoryComponent::UInventoryComponent()
	: MaxBagCount(3)
	, CurrentWeight(0.0f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedOwnerCharacter = Cast<AGameCharacterBase>(GetOwner());
	
	// TODO : [TEST] 현재 '플레이어 기본 인벤토리' 를 아래 의미합니다. 다만 이후 Table 화 등으로 인해 구분 및
	// 게임 로드 기능이 추가된다면 아래 부분을 제거하면 됩니다.
	UBagItemBase* BagItem = NewObject<UBagItemBase>(GetOwner());
	Bags.Emplace(BagItem);

	CalculateWeight();
}

FItemEncodedInfo* UInventoryComponent::GetItem(const FItemEncodedInfo& InItemEncodedInfo)
{
	for (UBagItemBase* Bag : Bags)
	{
		if (IsValid(Bag))
		{
			if (FItemEncodedInfo* FoundItem = Bag->GetItem(InItemEncodedInfo))
			{
				return FoundItem;
			}
		}
	}

	return nullptr;
}

void UInventoryComponent::TryGetItem(const FItemEncodedInfo& ItemToWant, int32 OriginItemOwnerUID)
{
	if (Bags.IsValidIndex(CurrentBagIndex))
	{
		EGetItemError Result = Bags[CurrentBagIndex]->CanGetItem(ItemToWant);
		if (Result == EGetItemError::NoError)
		{
			// 온라인 게임도 아니고 핵 뭐 알아서 하라 해서 클라이언트에서 아이템 알아서 먹고 서버에서 알아서 먹고...
			ServerGetItem(CurrentBagIndex, ItemToWant, OriginItemOwnerUID);
			Bags[CurrentBagIndex]->TryAddItem(ItemToWant);
			OnPlayerItemListChanged.Broadcast();
		}
	}
}

void UInventoryComponent::ServerGetItem_Implementation(int32 ClientBagIndex, const FItemEncodedInfo& ItemToWant, int32 OriginItemOwnerUID)
{
	// 온라인 게임도 아니고 핵 뭐 알아서 하라 해서 클라이언트에서 아이템 알아서 먹고 서버에서 알아서 먹고...
	if (Bags.IsValidIndex(ClientBagIndex) && Bags[ClientBagIndex])
	{
		CurrentBagIndex = ClientBagIndex;
		EGetItemError Result = Bags[CurrentBagIndex]->CanGetItem(ItemToWant);
		if (Result == EGetItemError::NoError)
		{
			if (CachedOwnerCharacter && CachedOwnerCharacter->HasAuthority() == false)
			{
				Bags[ClientBagIndex]->TryAddItem(ItemToWant);
			}
			
			TScriptInterface<IPAActor> OriginContainerActor = UUIDContainer::GetInstance(this)->GetObjectByUID(EUIDType::ItemContainer, OriginItemOwnerUID);
			if (OriginContainerActor != nullptr)
			{
				if (IItemContainer* OriginContainer = Cast<IItemContainer>(OriginContainerActor.GetObject()))
				{
					OriginContainer->RemoveItem(ItemToWant);
				}	
			}
		}
	}
}

bool UInventoryComponent::RemoveItem(const FItemEncodedInfo& ItemToRemove)
{
	for (UBagItemBase* BagItem : Bags)
	{
		if (BagItem && BagItem->TryToRemoveItem(ItemToRemove))
		{
			OnPlayerItemListChanged.Broadcast();
			CalculateWeight();
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::RemoveItem(int8 BagIndex, const FItemEncodedInfo& ItemEncodedInfo)
{
	if (Bags.IsValidIndex(BagIndex) == false || IsValid(Bags[BagIndex]) == false)
	{
		return false;
	}

	bool bRemoveSuccess =  Bags[BagIndex]->TryToRemoveItem(ItemEncodedInfo);
	return bRemoveSuccess;
}

void UInventoryComponent::UseItem(const FItemEncodedInfo& ItemEncodedInfo, const int32 OriginOwnerUID)
{
	ServerUseItem(ItemEncodedInfo, OriginOwnerUID);
}

void UInventoryComponent::ServerUseItem_Implementation(const FItemEncodedInfo& ItemEncodedInfo, const int32 OriginOwnerUID)
{
	if (IsValid(CachedOwnerCharacter))
	{
		UAbilityItemPayloads* PayloadObject = NewObject<UAbilityItemPayloads>();
		PayloadObject->ItemInfo = ItemEncodedInfo;
		PayloadObject->OwnerUID = OriginOwnerUID;

		FGameplayEventData EventPayload;
		EventPayload.EventTag = UseItemDirectlyTag;
		EventPayload.OptionalObject = PayloadObject;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CachedOwnerCharacter, EventPayload.EventTag, EventPayload);
	}
}

int32 UInventoryComponent::HaveItem(const FItemEncodedInfo& ItemEncodedInfo)
{
	for (int i = 0; i < Bags.Num(); ++i)
	{
		if (Bags[i]->HaveItem(ItemEncodedInfo.GetItemKey()))
		{
			return i;
		}
	}
	return INDEX_NONE;
}

bool UInventoryComponent::HaveItem(const FName& ItemKeyToFind)
{
	for (UBagItemBase* Bag : Bags)
	{
		if (Bag->HaveItem(ItemKeyToFind))
		{
			return true;
		}
	}
	return false;
}

void UInventoryComponent::CalculateWeight()
{
	CurrentWeight = 0.f;
	
	for (const UBagItemBase* Bag : Bags)
	{
		if (Bag)
		{
			CurrentWeight += Bag->GetItemsWeight(); 
		}
	}

	OnWeightChanged.Broadcast(CurrentWeight);
}
