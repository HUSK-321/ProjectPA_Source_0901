// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "EncodedItemListViewElement.generated.h"

struct FItemEncodedInfo;
class UImage;
class UTextBlock;

UCLASS()
class PROJECTPA_API UEncodedItemListViewElement : public UPAWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	bool bIsPlayerElement = true;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	FName ItemKey;
	
public:
	void SetElement(const FItemEncodedInfo& InItemInfo);

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	FORCEINLINE bool IsPlayerSide() const { return bIsPlayerElement; }
	FORCEINLINE FName GetItemKey() const { return ItemKey; }
};
