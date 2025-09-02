// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "ProjectPA/Generals/PATypes.h"
#include "EncodedItemListView.generated.h"

class UEncodedItemListViewElement;

UCLASS()
class PROJECTPA_API UEncodedItemListView : public UListView
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void SetItemList(const TArray<FItemEncodedInfo>& InEncodedItemList);
};
