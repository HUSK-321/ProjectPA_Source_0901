#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "ItemContextButtonWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class PROJECTPA_API UItemContextButtonWidget : public UPAWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text;
};
