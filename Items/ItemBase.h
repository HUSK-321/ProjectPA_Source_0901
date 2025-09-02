#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectPA/Generals/PATypes.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

struct FItemDataTable;
class UTexture2D;
class AGameCharacterBase;
enum class EItemType : uint8;
class AItemActorBase;

UCLASS(Blueprintable, BlueprintType)
class PROJECTPA_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	FSimpleMulticastDelegate OnItemInfoSet;

protected:
	UPROPERTY()
	TObjectPtr<AGameCharacterBase> Owner;
	
	UPROPERTY(EditAnywhere)
	EItemType ItemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	float Weight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Item")
	float ItemRemainPercentage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	bool bIsUsable;

	uint8 bDataInitialized : 1;
	uint8 bEquip : 1;
	uint8 bWear : 1;

private:
	// Item's Information
	UPROPERTY(ReplicatedUsing = OnRep_ItemEncodedInfo)
	FItemEncodedInfo ItemEncodedInfo;

public:
	UItemBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void InitializeItemInfo(const FItemDataTable* DataTableInfo);

	/**
	 * @brief 가방 내에서 사용할 수도 있고, 아니면 장착한 슬롯에서 사용할 수도 있다
	 */
	virtual void UseItem(AGameCharacterBase* InUser) {}
	virtual void EndUseItem(float Percentage, UAbilitySystemComponent* UserAttributeSystem) {}
	
	virtual void DropItem(AActor* InUser, FVector InDesiredLocation) {}

	virtual void EquipItem(AGameCharacterBase* InUser);
	virtual void WearItem(AGameCharacterBase* InUser);
	virtual void UnEquipItem(AGameCharacterBase* InUser);
	virtual void UnWearItem(AGameCharacterBase* InUser);

	FORCEINLINE UTexture2D* GetItemIcon() const { return ItemIcon.IsNull() == false ? ItemIcon.LoadSynchronous() : nullptr; }
	FORCEINLINE FText GetItemName() const { return ItemName; }
	FORCEINLINE FText GetItemDescription() const { return ItemDescription; } 
	FORCEINLINE EItemType GetItemType() const { return ItemType; }
	FORCEINLINE float GetWeight() const { return Weight; }
	FORCEINLINE bool IsUsable() const { return bIsUsable; }

	FORCEINLINE bool IsEquipped() const { return bEquip; }
	FORCEINLINE bool IsWeared() const { return bWear; }
	/**
	 * @brief 인벤토리 메뉴에서 사용 메뉴로 보여질 텍스트 (사용, 장착)
	 */
	virtual FText GetItemUseText() const { return FText(); }
	virtual UStaticMesh* GetMesh() const { return nullptr; }
	virtual FTransform GetAttachTransform() const { return FTransform::Identity; }

	FORCEINLINE void SetUsable(bool bUsable) { bIsUsable = bUsable; }

	void SetItemEncodedInfo(const FItemEncodedInfo& InEncodedInfo);
	bool IsSameItem(const FItemEncodedInfo& Compare) const;
	const FItemEncodedInfo& GetItemEncodedInfo() const { return ItemEncodedInfo; } 

protected:
	virtual void SetOwner(AGameCharacterBase* InOwner);
	virtual void LooseOwner();
	virtual bool IsBelongedToLocalPlayer() const;

	virtual void DestroyItem();

	UFUNCTION()
	void OnRep_ItemEncodedInfo();
};
