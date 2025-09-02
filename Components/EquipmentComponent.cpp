// All copyrights for this code are owned by Aster.


#include "EquipmentComponent.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/GameState.h"
#include "Net/UnrealNetwork.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/GameStates/WeatherSystemComponent.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"
#include "ProjectPA/Interfaces/ItemContainer.h"
#include "ProjectPA/Items/ItemDataTables.h"
#include "ProjectPA/Network/UIDContainer.h"

UEquipmentComponent::UEquipmentComponent()
	: MaxEquipSlotNum(3)
	, CurrentSlotIndex(INDEX_NONE)
	, EquippedWeight(0)
	, WearingWeight(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	EquipSlots.AddZeroed(MaxEquipSlotNum);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	OwnerCharacter = Cast<AGameCharacterBase>(GetOwner());

	if (TemperatureDeviationAttribute.IsValid() && GetWorld())
	{
		if (const AGameState* PlayGameState = GetWorld()->GetGameState<AGameState>())
		{
			if (UWeatherSystemComponent* WeatherSystemComponent = PlayGameState->GetComponentByClass<UWeatherSystemComponent>())
			{
				GetWorldTemperatureDelegate.BindUObject(WeatherSystemComponent, &UWeatherSystemComponent::GetCurTemperature);
				WeatherSystemComponent->OnTemperatureChanged.AddUObject(this, &ThisClass::OnTemperatureChanged);
			}
		}
	}
	
	// TODO : Load and save item infos
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UEquipmentComponent, EquipSlots, COND_OwnerOnly);
	DOREPLIFETIME(UEquipmentComponent, CurrentSlotIndex);
	DOREPLIFETIME(UEquipmentComponent, EquippingItem);
	DOREPLIFETIME(UEquipmentComponent, Wearings);
}

// Slot Functions
void UEquipmentComponent::ActiveForwardSlotIndex()
{
	int32 StartIndex = (CurrentSlotIndex - 1 + MaxEquipSlotNum) % MaxEquipSlotNum;
	int32 NewSlotIndex = StartIndex;

	while (NewSlotIndex != CurrentSlotIndex)
	{
		if (EquipSlots.IsValidIndex(NewSlotIndex) && EquipSlots[NewSlotIndex].IsValid())
		{
			ServerActiveSlotIndex(NewSlotIndex);
			return;
		}

		NewSlotIndex = (NewSlotIndex - 1 + MaxEquipSlotNum) % MaxEquipSlotNum;
	}
}

void UEquipmentComponent::ActiveBackwardSlotIndex()
{
	int32 StartIndex = (CurrentSlotIndex + 1) % MaxEquipSlotNum;
	int32 NewSlotIndex = StartIndex;
    
	while (NewSlotIndex != CurrentSlotIndex)
	{
		if (EquipSlots.IsValidIndex(NewSlotIndex) && EquipSlots[NewSlotIndex].IsValid())
		{
			ServerActiveSlotIndex(NewSlotIndex);
			return;
		}
        
		NewSlotIndex = (NewSlotIndex + 1) % MaxEquipSlotNum;
	}
}

void UEquipmentComponent::ServerActiveSlotIndex_Implementation(int32 NewSlotIndex)
{
	if (EquipSlots.IsValidIndex(NewSlotIndex) == false || EquipSlots[NewSlotIndex].IsValid() == false)
	{
		return;
	}

	CurrentSlotIndex = NewSlotIndex;
	EquippingItem = EquipSlots[NewSlotIndex];
}

// Equip Functions
bool UEquipmentComponent::CanEquipItem(const FItemEncodedInfo& ItemToEquip)
{
	bool CanEquipItem = true;
	for (const FItemEncodedInfo& Slot : EquipSlots)
	{
		if (Slot == ItemToEquip)
		{
			CanEquipItem = false;
		}
	}
	return IsAllSlotsUsed() == false && CanEquipItem;
}

void UEquipmentComponent::EquipItem(const FItemEncodedInfo& ItemToEquip, const int32 ItemOwnerUID,
	const int32 SlotIndex)
{
	// INDEX_NONE 는 가능한 가장 빠른 Slot 을 의미합니다.
	if (EquipSlots.IsValidIndex(SlotIndex) == false && SlotIndex != INDEX_NONE)
	{
		return;
	}
	
	if (OwnerCharacter && OwnerCharacter->GetUID() == ItemOwnerUID)
	{
		OwnerCharacter->RemoveItem(ItemToEquip);
	}
	ServerEquipItem(ItemToEquip, ItemOwnerUID, SlotIndex);
}

void UEquipmentComponent::ServerEquipItem_Implementation(const FItemEncodedInfo& ItemToEquip, const int32 ItemOwnerUID,
	const int32 SlotIndex)
{
	// INDEX_NONE 는 가능한 가장 빠른 Slot 을 의미합니다.
	if (EquipSlots.IsValidIndex(SlotIndex) == false && SlotIndex != INDEX_NONE)
	{
		return;
	}
	
	const int32 IndexToInsert = SlotIndex == INDEX_NONE ? GetFirstEmptySlotIndex() : SlotIndex;
	if (EquipSlots.IsValidIndex(IndexToInsert) == false)
	{
		return;
	}

	// TODO : 기존 아이템을 인벤토리로 넣는 작업
	if (EquipSlots[IndexToInsert].IsValid())
	{
	}
	
	RemoveItemFromOwner(ItemToEquip, ItemOwnerUID);
	
	EquipSlots[IndexToInsert] = ItemToEquip;
	OnRep_EquipSlots();

	if (CurrentSlotIndex == INDEX_NONE)
	{
		CurrentSlotIndex = SlotIndex;
	}
	
	if (SlotIndex == CurrentSlotIndex)
	{
		EquippingItem = EquipSlots[IndexToInsert];
		OnRep_EquippingItem();
	}
}

void UEquipmentComponent::UnEquipItem(const int32 SlotIndex)
{
	if (EquipSlots.IsValidIndex(SlotIndex) == false)
	{
		return;
	}
	if (EquipSlots[SlotIndex].IsValid() == false)
	{
		return;
	}

	ServerUnEquipItem(SlotIndex);
}

void UEquipmentComponent::ServerUnEquipItem_Implementation(const int32 SlotIndex)
{
	if (EquipSlots.IsValidIndex(SlotIndex) == false)
	{
		return;
	}
	
	if (EquipSlots[SlotIndex].IsValid() == false)
	{
		return;
	}

	// TODO : 기존 아이템을 인벤토리로 넣는 작업 or 자리 없을 경우 버리기 (이건 인벤토리에서 하자
	
	EquipSlots[SlotIndex] = FItemEncodedInfo();
	OnRep_EquipSlots();

	if (SlotIndex == CurrentSlotIndex)
	{
		EquippingItem = EquipSlots[SlotIndex];
		OnRep_EquippingItem();
	}
}

FItemDataTable* UEquipmentComponent::GetEquippingItemData() const
{
	return GetItemInfo(EquippingItem);	
}

void UEquipmentComponent::GetAllEquippedAndWornItems(TArray<FItemEncodedInfo>& OutItems) const
{
	OutItems.Append(EquipSlots);
	OutItems.Append(Wearings);
}

void UEquipmentComponent::ClearAllEquipment()
{
	EquipSlots.Empty();
	EquipSlots.AddZeroed(MaxEquipSlotNum);
	
	Wearings.Empty();

	EquippingItem = FItemEncodedInfo();
	CurrentSlotIndex = INDEX_NONE;
	
	OnRep_EquippingItem();
	OnRep_EquipSlots();
	OnRep_Wearings();
}

void UEquipmentComponent::DestroyEquippedItem()
{
	
}

// Wearing Functions
void UEquipmentComponent::WearItem(const FItemEncodedInfo& ItemToWear, const int32 ItemOwnerUID)
{
	// Remove item by actor uid
	if (OwnerCharacter && OwnerCharacter->GetUID() == ItemOwnerUID)
	{
		OwnerCharacter->RemoveItem(ItemToWear);
	}

	ServerWearItem(ItemToWear, ItemOwnerUID);
}

void UEquipmentComponent::ServerWearItem_Implementation(const FItemEncodedInfo& ItemToWear, const int32 ItemOwnerUID)
{
	RemoveItemFromOwner(ItemToWear, ItemOwnerUID);

	if (FItemDataTable* ItemData = GetItemInfo(ItemToWear))
	{
		if (ItemData->ItemType == EItemType::Wearing)
		{
			FWearingItemDataTable* WearingItem = static_cast<FWearingItemDataTable*>(ItemData);

			const uint8 ItemCategory = WearingItem->WearCategory;
			const EWearCategory ItemWearCategory = static_cast<EWearCategory>(ItemCategory);
			
			for (uint8 Index = 0; Index < WearCategoryCount; ++Index)
			{
				EWearCategory Category = static_cast<EWearCategory>(1 << Index);

				if (EnumHasAnyFlags(ItemWearCategory, Category))
				{
					// TODO : 배낭으로 아이템 돌리기
					Wearings[Index] = ItemToWear;
				}
			}

			WearingWeight += ItemData->Weight;
			OnRep_Wearings();
		}
	}
}

void UEquipmentComponent::UnWearItem(EWearCategory Category)
{
	ServerUnWearItem(Category);
}

void UEquipmentComponent::ServerUnWearItem_Implementation(EWearCategory Category)
{
	uint8 TempCategory = static_cast<uint8>(Category);
	FName UnWearedItemName;
	
	while (TempCategory > 0)
	{
		uint8 Index = FMath::CountTrailingZeros(TempCategory);

		if (Wearings.IsValidIndex(Index))
		{
			UnWearedItemName = Wearings[Index].GetItemKey();
			Wearings[Index] = FItemEncodedInfo();
		}
		
		TempCategory &= (TempCategory - 1);
	}

	if (UnWearedItemName.IsValid())
	{
		if (const FItemDataTable* ItemData = GetItemInfo(UnWearedItemName))
		{
			// TODO: 아이템을 인벤토리로 되돌리는 로직
			WearingWeight -= ItemData->Weight;
		}
	}

    
	OnRep_Wearings();
}

static const FItemEncodedInfo EmptySlot = FItemEncodedInfo();

const FItemEncodedInfo& UEquipmentComponent::GetWearingItem(EWearCategory Category) const
{
	const uint8 Index = FMath::FloorLog2(static_cast<uint8>(Category));
	if (Wearings.IsValidIndex(Index))
	{
		return Wearings[Index];
	}

	return EmptySlot;
}

const FItemEncodedInfo& UEquipmentComponent::GetWearingItem_Ref(EWearCategory Category)
{
	const uint8 Index = FMath::FloorLog2(static_cast<uint8>(Category));
	if (Wearings.IsValidIndex(Index))
	{
		return Wearings[Index];
	}

	return EmptySlot;
}

// Replicates
void UEquipmentComponent::OnRep_EquipSlots()
{
	for (auto& Slot : EquipSlots)
	{
		
	}
	
	// UI Setting
}

void UEquipmentComponent::OnRep_CurrentSlotIndex()
{
	// Index 와 Equipping Item 이 맞지 않는다면?
	if (EquippingItem != EquipSlots[CurrentSlotIndex])
	{
		
	}
	// TODO : Cancel Use Item Ability
}

void UEquipmentComponent::OnRep_EquippingItem()
{
	const FItemDataTable* ItemData = GetItemInfo(EquippingItem);
	if (ItemData)
	{
		const TSoftObjectPtr<UStaticMesh>& ItemMeshSoft = ItemData->GetItemMesh();
		// TODO : Item Equip Transform
		UStaticMesh* ItemMesh = (ItemMeshSoft.IsNull() == false) ? ItemMeshSoft.LoadSynchronous() : nullptr;
		SetStaticMesh(ItemMesh);
		SetRelativeTransform(ItemData->GetItemAttachTransform());
	}

	// TODO : Cancel Use Item Ability
}

void UEquipmentComponent::OnRep_Wearings()
{
	constexpr float DummyFloat = -1.0f;
	OnTemperatureChanged(DummyFloat);
}

// Temperatures
void UEquipmentComponent::OnTemperatureChanged(const float CurTemperature)
{
	const float Deviation = GetWearingWarmth() + GetTemperature();

	if (OwnerCharacter && OwnerCharacter->GetAbilitySystemComponent())
	{
		OwnerCharacter->GetAbilitySystemComponent()->SetNumericAttributeBase(TemperatureDeviationAttribute, Deviation);
	}
}

float UEquipmentComponent::GetWearingWarmth() const
{
	float TotalWarmth = 0.0f;
	for (const auto WearingSlot : Wearings)
	{
		const FWearingItemDataTable* WearingItemData = static_cast<FWearingItemDataTable*>(GetItemInfo(WearingSlot.GetItemKey()));
		TotalWarmth += WearingItemData->ClothingWarmth; 
	}
	return TotalWarmth;
}

float UEquipmentComponent::GetTemperature() const
{
	return GetWorldTemperatureDelegate.IsBound() ? GetWorldTemperatureDelegate.Execute() : -999.0f;
}

// Utils
int32 UEquipmentComponent::GetFirstEmptySlotIndex()
{
	for (int32 i = 0; i < EquipSlots.Num(); ++i)
	{
		if (EquipSlots[i].IsValid() == false)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

bool UEquipmentComponent::IsAllSlotsUsed() const
{
	for (const FItemEncodedInfo& Slot : EquipSlots)
	{
		if (Slot.IsValid() == false)
		{
			return false;
		}
	}
	return true;
}

void UEquipmentComponent::RemoveItemFromOwner(const FItemEncodedInfo& ItemToWear, const int32 ItemOwnerUID)
{
	if (OwnerCharacter && OwnerCharacter->GetUID() == ItemOwnerUID)
	{
		OwnerCharacter->RemoveItem(ItemToWear);
	}
	else if (TScriptInterface<IPAActor> ItemOwnerActor = UUIDContainer::GetInstance(this)->GetObjectByUID(ItemOwnerUID))
	{
		if (IItemContainer* ItemContainer = Cast<IItemContainer>(ItemOwnerActor.GetObject()))
		{
			ItemContainer->RemoveItem(ItemToWear);
		}
	}
}

FItemDataTable* UEquipmentComponent::GetItemInfo(const FName& ItemKey) const
{
	IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>();
	check(DataProvider);
	return DataProvider->GetItemDataByKey(ItemKey);
}

FItemDataTable* UEquipmentComponent::GetItemInfo(const FItemEncodedInfo& ItemInfo) const
{
	return GetItemInfo(ItemInfo.GetItemKey());
}
