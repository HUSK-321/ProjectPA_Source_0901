#include "ItemContextMenuWidget.h"
#include "Components/VerticalBox.h"

void UItemContextMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemContextMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	ClearAndHide();
	
	if (OnContextWidgetMouseLeaved.IsBound())
	{
		OnContextWidgetMouseLeaved.Execute();
	}
}

void UItemContextMenuWidget::ClearAndHide()
{
	if (ButtonBox)
	{
		ButtonBox->ClearChildren();
	}

	SetVisibility(ESlateVisibility::Collapsed);
}
