// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "ItemStatWidgetBase.generated.h"

struct FItemEncodedInfo;
struct FItemDataTable;
class UItemBase;

/**
 * 아이템 세부 내용 위젯(UItemTooltipWidget) 에서 교체되면서 사용하게 될 아이템의 상세 스탯을 표기하는 위젯
 */
UCLASS()
class PROJECTPA_API UItemStatWidgetBase : public UPAWidget
{
	GENERATED_BODY()

public:
	virtual void SetItemStatus(const FItemDataTable* InItemData, const FItemEncodedInfo& InItemInfo) {}
};
