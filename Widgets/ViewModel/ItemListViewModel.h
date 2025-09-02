// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "PABaseViewModel.h"
#include "ProjectPA/Interfaces/ItemContainer.h"
#include "ItemListViewModel.generated.h"

class UBagItemBase;
class UItemEncodedInfoObject;
class UInteractionComponent;
class UInventoryComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnContainerListChanged, const TArray<UTexture2D*>&);

/**
 * 아이템 목록(플레이어 아이템, 근처 아이템)을 위한 ViewModel 클래스입니다.
 * 인벤토리 관련 데이터와 UI 로직을 관리합니다.
 */
UCLASS(BlueprintType)
class PROJECTPA_API UItemListViewModel : public UPABaseViewModel
{
    GENERATED_BODY()

public:
	FOnContainerListChanged OnPlayerBagCountChanged;
	FOnContainerListChanged OnNearbyListCountChanged;

private:
	int32 OwnerUID;
	UPROPERTY()
	TObjectPtr<UInteractionComponent> InteractionComponent;
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	//~Begin Item Widget
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, meta = (AllowPrivateAccess = true))
	FText PlayerInventoryWeightText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	float PlayerInventoryWeight;
	
	UPROPERTY()
	TArray<UBagItemBase*> PlayerBags;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, meta = (AllowPrivateAccess = true))
	TArray<UObject*> DisplayedPlayerItems;

	UPROPERTY()
	TArray<TScriptInterface<IItemContainer>> NearbyContainers;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, meta = (AllowPrivateAccess = true))
	TArray<UObject*> DisplayedNearbyItems;
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	int32 CurPlayerBagIndex;
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true))
	int32 CurNearbyListIndex;
	//~End Item Widget

	//~Begin ToolTip and Context
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, meta = (AllowPrivateAccess = true))
	FItemEncodedInfo HoveredItemInfo;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, meta = (AllowPrivateAccess = true))
	FItemEncodedInfo ContextMenuItemInfo;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter = "GetCurContextIsPLayerSide", meta = (AllowPrivateAccess = true))
	bool CurContextIsPlayerSide;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter = "GetIsContextMenuVisible", meta = (AllowPrivateAccess = true))
	bool bIsContextMenuVisible;
	//~End ToolTip and Context

public:
	UItemListViewModel();

	virtual void Initialize(UObject* OwningObject) override;
	
	void SetPlayerInventoryWeight(const float NewWeight)
	{
		UE_MVVM_SET_PROPERTY_VALUE(PlayerInventoryWeight, NewWeight);
	}
	float GetPlayerInventoryWeight() const { return PlayerInventoryWeight; }
	
	FText GetPlayerInventoryWeightText() const;

	void SetPlayerBags(const TArray<UBagItemBase*>& NewPlayerBags);
	const TArray<UObject*>& GetDisplayedPlayerItems() const { return DisplayedPlayerItems; }

	void SetNearbyContainers(const TArray<TScriptInterface<IItemContainer>>& NewNearbyContainers);
	UFUNCTION(BlueprintPure, FieldNotify)
	const TArray<UObject*>& GetDisplayedNearbyItems() const { return DisplayedNearbyItems; }

	void SetCurPlayerBagIndex(const int32 NewIndex)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurPlayerBagIndex, NewIndex);

		UpdateDisplayedPlayerItems();
	}
	int32 GetCurPlayerBagIndex() const { return CurPlayerBagIndex; }

	void SetCurNearbyListIndex(const int32 NewIndex)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurNearbyListIndex, NewIndex);

		UpdateDisplayedNearbyContainers();
	}
	int32 GetCurNearbyListIndex() const { return CurNearbyListIndex; }

	void RequestUseContextItem();
	
	void RequestRemoveContextItem();
	
	void RequestAcquireContextItem();

	//~Begin Tooltips
	void HandleItemRightClick(const FItemEncodedInfo& ItemInfo, bool bIsPlayerSide);
	
	void HandleItemHover(const FItemEncodedInfo& ItemInfo, bool bIsPlayerSide);

	void HandleItemUnhover(const FItemEncodedInfo& ItemInfo, bool bIsPlayerSide);

	UFUNCTION(BlueprintCallable)
	void ClearContextMenu();
	
	FItemEncodedInfo GetHoveredItemInfo() const { return HoveredItemInfo; }
	bool GetIsContextMenuVisible() const { return bIsContextMenuVisible; }
	FItemEncodedInfo GetContextMenuItemInfo() const { return ContextMenuItemInfo; }
	bool GetCurContextIsPLayerSide() const { return CurContextIsPlayerSide; }
	//~End Tooltips

private:
	/** 플레이어의 아이템들이 변경될 때 불리게 될 함수 */
	void UpdateDisplayedPlayerItems();
    UItemEncodedInfoObject* MakeItemEncodedObject(const FItemEncodedInfo& ItemInfo);
    /** 근처 아이템 목록이 변경될 때 불리게 될 함수 */
	void UpdateDisplayedNearbyContainers();

	void OnPlayerItemListChanged();
	void OnNearbyItemListChanged();
	void OnWeightChanged(const float NewWeight);

	int32 GetContextItemOwnerUID() const;
};
