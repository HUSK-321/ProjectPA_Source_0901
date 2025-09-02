// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "ItemTooltipWidget.generated.h"

class UVerticalBox;
class UMultiLineEditableTextBox;
class UItemBase;
class UTextBlock;
class UItemStatWidgetBase;
class UItemEncodedInfoObject;
enum class EItemType : uint8;

/**
 * 아이템 인벤토리 위젯에서 아이템에 호버시 보여주게 될 아이템의 세부 내용 위젯 (ToolTip)
 */
UCLASS()
class PROJECTPA_API UItemTooltipWidget : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> ItemDescription;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> WeightText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> ItemToolTipBox;
	
	UPROPERTY(EditAnywhere);
	TMap<EItemType, UItemStatWidgetBase*> StatusWidgets;
	EItemType CurTooltipItemType;

public:
	virtual void NativeConstruct() override;
	void SetTooltipWidget(const FItemEncodedInfo& ItemInfo);

private:
	void CacheStatusWidgets();
};
