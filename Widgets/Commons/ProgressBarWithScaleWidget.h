// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "ProgressBarWithScaleWidget.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECTPA_API UProgressBarWithScaleWidget : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CurrentValue;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MaxValue;

public:
	/**
	 * @param InCurrentValue 현재 값
	 * @param InMaxValue 최대 값
	 * @param bDecimal 소수점 자리를 나타낼지 여부 (true : 5.0, false : 5)
	 */
	void SetValue(const float InCurrentValue, const float InMaxValue, bool bDecimal = false);
};
