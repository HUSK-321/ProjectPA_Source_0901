// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ItemStatWidgetBase.h"
#include "AttributeSet.h"
#include "ConsumeItemStatWidget.generated.h"

struct FItemEncodedInfo;
enum class ESurvivalStats : uint8;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTPA_API UConsumeItemStatWidget : public UItemStatWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ExpireDate;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayAttribute, UTextBlock*> StatTexts;
	
public:
	virtual void NativeConstruct() override;
	virtual void SetItemStatus(const FItemDataTable* InItemData, const FItemEncodedInfo& InItemInfo) override;

private:
	void CacheStatTexts();
};
