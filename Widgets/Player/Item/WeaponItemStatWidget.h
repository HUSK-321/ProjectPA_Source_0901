// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ItemStatWidgetBase.h"
#include "WeaponItemStatWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTPA_API UWeaponItemStatWidget : public UItemStatWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DamageText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RangeText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AngleText;

public:
	virtual void SetItemStatus(const FItemDataTable* InItemData, const FItemEncodedInfo& InItemInfo) override;
};
