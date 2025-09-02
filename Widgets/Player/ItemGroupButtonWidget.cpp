// All copyrights for this code are owned by Aster.


#include "ItemGroupButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UItemGroupButtonWidget::SetButtonClickedEvent(const int32 ButtonIndex, const FOnItemGroupButtonClicked& OnClickedCallback, UTexture2D* InButtonIcon)
{
	if (IsValid(Button) == false)
	{
		return;
	}
	
	GroupIndex = ButtonIndex;
	OnButtonClickedDelegate = OnClickedCallback;
	
	Button->OnClicked.Clear();
	Button->OnClicked.AddDynamic(this, &UItemGroupButtonWidget::OnButtonClicked);

	if (IsValid(InButtonIcon) && IsValid(ButtonIcon))
	{
		ButtonIcon->SetBrushFromTexture(InButtonIcon);
	}
}

void UItemGroupButtonWidget::OnButtonClicked()
{
	if (OnButtonClickedDelegate.IsBound())
	{
		OnButtonClickedDelegate.Execute(GroupIndex);
	}
}
