// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "ProjectPA/Items/BagItemBase.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "PlayerInventoryWidget.generated.h"

class UEquipmentComponent;
class UInventoryComponent;
class APlayerCharacter;
class UEncodedItemListView;
class UButton;
class UTextBlock;
class UItemContextButtonWidget;
class UItemContextMenuWidget;
class UItemTooltipWidget;
class UItemGroupButtonWidget;
class UEncodedItemListViewElement;
class UHorizontalBox;
class UBagItemBase;
class IItemContainer;
class UItemListViewModel;

USTRUCT()
struct FItemContainerList
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UTexture2D> ContainerImage;
};

UCLASS()
class PROJECTPA_API UPlayerInventoryWidget : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UI", meta=(AllowPrivateAccess=true))
	TSubclassOf<UItemContextButtonWidget> ContextMenuButtonClass;
	UPROPERTY(EditAnywhere, Category = "UI", meta=(AllowPrivateAccess=true))
	TSubclassOf<UItemContextMenuWidget> ContextMenuWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI", meta=(AllowPrivateAccess=true))
	TSubclassOf<UItemTooltipWidget> ItemTooltipWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI", meta=(AllowPrivateAccess=true))
	TSubclassOf<UItemGroupButtonWidget> ItemCategoryButtonWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI", meta=(AllowPrivateAccess=true))
	FMargin ButtonBoxPadding;
	UPROPERTY(EditAnywhere, Category = "UI", meta=(AllowPrivateAccess=true))
	FVector2D ContextMenuPositionOffset;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UHorizontalBox> BagButtonBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UHorizontalBox> NearByButtonBox;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> WeightText;

	/** Context Widget */
	UPROPERTY()
	TObjectPtr<UItemContextMenuWidget> CachedContextWidget;

	/** Tooltip Widget */
	UPROPERTY()
	TObjectPtr<UItemTooltipWidget> CachedTooltipWidget;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta=(AllowPrivateAccess=true))
	TObjectPtr<UItemListViewModel> ItemListViewModel;
	
public:
	virtual void NativeConstruct() override;

	void OnPlayerBagCountChanged(const TArray<UTexture2D*>& BagImageList);
	
	void OnNearbyListCountChanged(const TArray<UTexture2D*>& NearbyImageList);
	
	void SetButtonBoxChildNum(UHorizontalBox* TargetButtonBox, const TArray<UTexture2D*>& ContainerImageList, bool bPLayerSide);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void HandleToolTipWidgetChanged(const FItemEncodedInfo& ItemInfo);	
	
	/** Functions for ListView's Elements / Context Menu */
	void CreateContextMenu(const FItemEncodedInfo& ItemInfo, bool bIsPlayerSide);
	void MakeContextMenuButtons();
	void ClearContextMenu();

	/** Function to bind cpp's ViewModel to BP's ViewModel */
	UFUNCTION(BlueprintImplementableEvent)
	void OnViewModelBinded();
	
private:
	UFUNCTION()
	void OnAcquireButtonClicked();
	UFUNCTION()
	void OnUseItemButtonClicked();
	UFUNCTION()
	void OnRemoveItemButtonClicked();
	
	void OnBagButtonClicked(const int32 ButtonIndex);

	void OnNearbyGroupButtonClicked(const int32 ButtonIndex);

	UItemContextButtonWidget* MakeButtonWidget() const;
};
