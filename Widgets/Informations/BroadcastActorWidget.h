// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "BroadcastActorWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTPA_API UBroadcastActorWidget : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> BroadcastText;
	
public:
	void SetBroadcastText(const FText& InBroadcasterName, const FText& InBroadcastText);
};
