// All copyrights for this code are owned by Aster.


#include "PlayerInventoryWidget.h"
#include "EncodedItemListViewElement.h"
#include "ItemTooltipWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "ProjectPA/Widgets/WidgetTypes.h"
#include "ProjectPA/Widgets/Player/ItemContextButtonWidget.h"
#include "ProjectPA/Widgets/Player/ItemContextMenuWidget.h"
#include "ProjectPA/Widgets/Player/ItemGroupButtonWidget.h"
#include "ProjectPA/Widgets/SubSystem/PAViewModelUtils.h"
#include "ProjectPA/Widgets/ViewModel/ItemListViewModel.h"

void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ButtonBoxPadding = { 0.f, 0.f, 10.f, 0.f };
	ContextMenuPositionOffset = {150.0f, 50.0f};

	if (!ItemListViewModel)
	{
		PAViewModelUtils::BindCallbackOnViewModelAdded<UItemListViewModel>(this, 
			[this](UItemListViewModel* ViewModel)
			{
				if (ViewModel)
				{
					ItemListViewModel = ViewModel;
					if (ItemListViewModel)
					{
						ItemListViewModel->OnPlayerBagCountChanged.AddUObject(this, &UPlayerInventoryWidget::OnPlayerBagCountChanged);
						ItemListViewModel->OnNearbyListCountChanged.AddUObject(this, &UPlayerInventoryWidget::OnNearbyListCountChanged);
					}
					OnViewModelBinded();
				}
			});
	}
}

void UPlayerInventoryWidget::OnPlayerBagCountChanged(const TArray<UTexture2D*>& BagImageList)
{
	SetButtonBoxChildNum(BagButtonBox, BagImageList, true);
}

void UPlayerInventoryWidget::OnNearbyListCountChanged(const TArray<UTexture2D*>& NearbyImageList)
{
	SetButtonBoxChildNum(NearByButtonBox, NearbyImageList, false);
}

void UPlayerInventoryWidget::SetButtonBoxChildNum(UHorizontalBox* TargetButtonBox, const TArray<UTexture2D*>& ContainerImageList, const bool bPLayerSide)
{
	if (IsValid(TargetButtonBox) == false)
	{
		return;
	}
	
	const int32 OriginUICount = TargetButtonBox->GetChildrenCount();

	// Set Button Num by list count
	const int32 NewNum = ContainerImageList.Num();
	FOnItemGroupButtonClicked ClickedDelegate = bPLayerSide ?
												FOnItemGroupButtonClicked::CreateUObject(this, &UPlayerInventoryWidget::OnBagButtonClicked) :
												FOnItemGroupButtonClicked::CreateUObject(this, &UPlayerInventoryWidget::OnNearbyGroupButtonClicked);
	for (int32 Index = OriginUICount; Index < NewNum; ++Index)
	{
		if (UItemGroupButtonWidget* NewButton = CreateWidget<UItemGroupButtonWidget>(this, ItemCategoryButtonWidgetClass))
		{
			TargetButtonBox->AddChild(NewButton);
			NewButton->SetPadding(ButtonBoxPadding);

			NewButton->SetButtonClickedEvent(Index, ClickedDelegate, ContainerImageList[Index]);
		}
	}
	while (TargetButtonBox->GetChildrenCount() > NewNum)
	{
		UWidget* LastWidget = TargetButtonBox->GetChildAt(TargetButtonBox->GetChildrenCount() - 1);
		TargetButtonBox->RemoveChild(LastWidget);
	}
}

void UPlayerInventoryWidget::HandleToolTipWidgetChanged(const FItemEncodedInfo& ItemInfo)
{
	if (IsValid(CachedTooltipWidget) == false)
	{
		CachedTooltipWidget = CreateWidget<UItemTooltipWidget>(GetWorld(), ItemTooltipWidgetClass);
		if (CachedTooltipWidget == nullptr)
		{
			return;
		}
	}
	
	if (ItemInfo.IsValid() == false)
	{
		CachedTooltipWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		FVector2D MousePosition;
		GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y);
		MousePosition += FVector2D(10.f, 0.f);
		if (CachedTooltipWidget->IsInViewport() == false)
		{
			CachedTooltipWidget->AddToViewport();
		}
		CachedTooltipWidget->SetPositionInViewport(MousePosition);
		CachedTooltipWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		CachedTooltipWidget->SetTooltipWidget(ItemInfo);
	}
}

void UPlayerInventoryWidget::CreateContextMenu(const FItemEncodedInfo& ItemInfo, bool bIsPlayerSide)
{
	if (IsValid(CachedContextWidget) == false)
	{
		CachedContextWidget = CreateWidget<UItemContextMenuWidget>(GetWorld(), ContextMenuWidgetClass);
		// TODO : 해당 부분의 순서를 고려하라
		CachedContextWidget->OnContextWidgetMouseLeaved.BindUObject(this, &UPlayerInventoryWidget::ClearContextMenu);
	}
}

void UPlayerInventoryWidget::MakeContextMenuButtons()
{

}

// void UPlayerInventoryWidget::MakeNearbyItemButtons(UVerticalBox* ButtonBox)
// {
// 	if (UItemContextButtonWidget* NewButton = MakeButtonWidget())
// 	{
// 		NewButton->Text->SetText(FText::FromString(TEXT("획득")));
// 		NewButton->Button->OnClicked.AddDynamic(this, &UPlayerInventoryWidget::OnAcquireButtonClicked);
// 		ButtonBox->AddChildToVerticalBox(NewButton);
// 	}
// }

UItemContextButtonWidget* UPlayerInventoryWidget::MakeButtonWidget() const
{
	UItemContextButtonWidget* MakedWidget = nullptr;

	if (ContextMenuButtonClass)
	{
		MakedWidget = CreateWidget<UItemContextButtonWidget>(GetWorld(), ContextMenuButtonClass);
	}
	
	return MakedWidget;
}

void UPlayerInventoryWidget::ClearContextMenu()
{
	if (CachedContextWidget)
	{
		CachedContextWidget->ClearAndHide();
	}
}

void UPlayerInventoryWidget::OnAcquireButtonClicked()
{
	if (ItemListViewModel)
	{
		ItemListViewModel->RequestAcquireContextItem();
	}
}

void UPlayerInventoryWidget::OnUseItemButtonClicked()
{
	if (ItemListViewModel)
	{
		ItemListViewModel->RequestUseContextItem();
	}
}

void UPlayerInventoryWidget::OnRemoveItemButtonClicked()
{
	if (ItemListViewModel)
	{
		ItemListViewModel->RequestRemoveContextItem();
	}
}

void UPlayerInventoryWidget::OnBagButtonClicked(const int32 ButtonIndex)
{
	if (ItemListViewModel)
	{
		ItemListViewModel->SetCurPlayerBagIndex(ButtonIndex);
	}
}

void UPlayerInventoryWidget::OnNearbyGroupButtonClicked(const int32 ButtonIndex)
{
	if (ItemListViewModel)
	{
		ItemListViewModel->SetCurNearbyListIndex(ButtonIndex);
	}
}
