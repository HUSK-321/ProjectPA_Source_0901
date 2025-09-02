// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Items/ItemListTypes.h"
#include "UObject/Interface.h"
#include "ItemContainer.generated.h"

UINTERFACE()
class UItemContainer : public UInterface
{
	GENERATED_BODY()
};

struct FItemEncodedInfo;

class UItemBase;

/**
 * 아이템 파밍이 가능한 Interface
 */
class PROJECTPA_API IItemContainer
{
	GENERATED_BODY()

public:
	virtual const FText& GetItemActorName() const = 0;
	virtual UTexture2D* GetContainerIcon() const { return nullptr; }

	virtual FDelegateHandle AddItemChangedDelegate(FSimpleDelegate& InDelegate) { return FDelegateHandle(); }
	virtual void RemoveItemChangedDelegate(FDelegateHandle& InDelegateHandle) {}

	/** Call this function 'false' on BeginPlay() */
	virtual void SetOverlayEffect(const bool bEnable) {}

	virtual bool CanGetItemList() const { return false; }
	virtual void GetItemList(OUT TArray<FItemEncodedInfo>& OUtItemEncodedInfos) { }
	virtual FItemEncodedInfo* GetItem(const FItemEncodedInfo& ItemEncodedInfo) { return nullptr;}
	virtual bool HasItem(const FItemEncodedInfo& ItemEncodedInfo) { return false; }
	virtual void AddItem(const FItemEncodedInfo& ItemEncodedInfo) { }
	virtual void RemoveItem(const FItemEncodedInfo& ItemEncodedInfo) { }

	virtual int32 GetContainerUID() const { return INDEX_NONE; }
};
 