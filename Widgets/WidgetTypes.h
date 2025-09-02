// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Generals/PATypes.h"
#include "UObject/Object.h"
#include "WidgetTypes.generated.h"

class UItemListViewModel;

/**
 * UEncodedItemListViewElement 를 설정할 때 전달하게 되는 Object Item
 */
UCLASS(BlueprintType)
class UItemEncodedInfoObject : public UObject
{
	GENERATED_BODY()

public:
	UItemEncodedInfoObject(){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemEncodedInfo ItemInfo;
	UPROPERTY()
	TObjectPtr<UItemListViewModel> ItemListViewModel;
};