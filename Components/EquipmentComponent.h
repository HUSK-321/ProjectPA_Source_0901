// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ProjectPA/Generals/PATypes.h"
#include "EquipmentComponent.generated.h"

class AGameCharacterBase;
struct FItemDataTable;

DECLARE_EVENT_OneParam(UEquipmentComponent, FOnWarmthChanged, const float);
DECLARE_DELEGATE_RetVal(float, FGetWorldTemperatureDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType )
class PROJECTPA_API UEquipmentComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	FOnWarmthChanged OnWarmthChanged;

private:
	FGetWorldTemperatureDelegate GetWorldTemperatureDelegate;

	//~Begin Equip Items
	UPROPERTY(EditAnywhere, Category = Equipping)
	int32 MaxEquipSlotNum;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_EquipSlots, Category = Equipping)
	TArray<FItemEncodedInfo> EquipSlots;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_CurrentSlotIndex, Category = Equipping)
	int32 CurrentSlotIndex;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_EquippingItem, Category = Equipping)
	FItemEncodedInfo EquippingItem;

	float EquippedWeight;
	//~End Equip Items

	//~Begin Wear Items
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Wearings, Category = Wearing)
	TArray<FItemEncodedInfo> Wearings;

	float WearingWeight; 
	//~End Wear Items

	UPROPERTY(EditAnywhere, Category = "Temperature")
	FGameplayAttribute TemperatureDeviationAttribute;

	UPROPERTY()
	TObjectPtr<AGameCharacterBase> OwnerCharacter;
	

public:
	UEquipmentComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Slot Functions
	void ActiveForwardSlotIndex();
	
	void ActiveBackwardSlotIndex();

	// Equip Functions
	bool CanEquipItem(const FItemEncodedInfo& ItemToEquip);
	
	void EquipItem(const FItemEncodedInfo& ItemToEquip, const int32 ItemOwnerUID, const int32 SlotIndex = INDEX_NONE);
	
	void UnEquipItem(const int32 SlotIndex);

	void DestroyEquippedItem();

	// Wear Functions
	void WearItem(const FItemEncodedInfo& ItemToWear, const int32 ItemOwnerUID);

	void UnWearItem(EWearCategory Category);

	FORCEINLINE const FItemEncodedInfo& GetEquippingItem() const { return EquippingItem; }
	FORCEINLINE FItemEncodedInfo& GetEquippingItem_Ref() { return EquippingItem; }
	FORCEINLINE const FItemEncodedInfo& GetWearingItem(EWearCategory Category) const;
	FORCEINLINE const FItemEncodedInfo& GetWearingItem_Ref(EWearCategory Category);
	FORCEINLINE float GetEquippedWeight() const { return EquippedWeight; }
	FORCEINLINE float GetWearingWeight() const { return WearingWeight; }

	FItemDataTable* GetEquippingItemData() const;
	template<typename T>
	T* GetEquippingItemData() const { return static_cast<T*>(GetEquippingItemData()); }

	/** 장착 및 착용 중인 모든 아이템 정보를 배열에 추가합니다. */
	void GetAllEquippedAndWornItems(TArray<FItemEncodedInfo>& OutItems) const;

	/** 모든 장착 및 착용 슬롯을 비웁니다. (주로 캐릭터 사망 시 호출) */
	void ClearAllEquipment();
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(Reliable, Server)
	void ServerActiveSlotIndex(int32 NewSlotIndex);

	UFUNCTION(Reliable, Server)
	void ServerEquipItem(const FItemEncodedInfo& ItemToEquip, const int32 ItemOwnerUID, const int32 SlotIndex = INDEX_NONE);
	UFUNCTION(Reliable, Server)
	void ServerUnEquipItem(const int32 SlotIndex);

	UFUNCTION(Reliable, Server)
	void ServerWearItem(const FItemEncodedInfo& ItemToWear, const int32 ItemOwnerUID);
	UFUNCTION(Reliable, Server)
	void ServerUnWearItem(EWearCategory Category);

	UFUNCTION()
	void OnRep_EquipSlots();

	UFUNCTION()
	void OnRep_CurrentSlotIndex();

	UFUNCTION()
	void OnRep_EquippingItem();

	UFUNCTION()
	void OnRep_Wearings();

	void OnTemperatureChanged(float CurTemperature);
	float GetWearingWarmth() const;

	float GetTemperature() const;
	
	int32 GetFirstEmptySlotIndex();

	bool IsAllSlotsUsed() const;
	
	void RemoveItemFromOwner(const FItemEncodedInfo& ItemToWear, int32 ItemOwnerUID);

	FItemDataTable* GetItemInfo(const FName& ItemKey) const;
	FItemDataTable* GetItemInfo(const FItemEncodedInfo& ItemInfo) const;
};
