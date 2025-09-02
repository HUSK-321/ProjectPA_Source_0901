#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "ItemContextMenuWidget.generated.h"

class UVerticalBox;
class UItemContextButtonWidget;
class UItemBase;

UCLASS()
class PROJECTPA_API UItemContextMenuWidget : public UPAWidget
{
	GENERATED_BODY()

public:
	/** 해당 위젯은 상위 위젯 (~Element) 에서들 관리하므로 해당 Delegate 를 통해 Widget 제거하도록 */
	FSimpleDelegate OnContextWidgetMouseLeaved;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> ButtonBox;

public:
	virtual void NativeConstruct() override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void ClearAndHide();
};
