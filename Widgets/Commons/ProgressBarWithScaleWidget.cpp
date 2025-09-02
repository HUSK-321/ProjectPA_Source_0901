// All copyrights for this code are owned by Aster.


#include "ProgressBarWithScaleWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UProgressBarWithScaleWidget::SetValue(const float InCurrentValue, const float InMaxValue, bool bDecimal)
{
	if (IsValid(CurrentValue) == false || IsValid(MaxValue) == false) [[unlikely]]
	{
	    return;
    }

	const FString CurrentValueString = bDecimal ? FString::Printf(TEXT("%.2f"), InCurrentValue) : FString::Printf(TEXT("%d"), static_cast<int32>(InCurrentValue));
	const FString MaxValueString = bDecimal ? FString::Printf(TEXT("%.2f"), InMaxValue) : FString::Printf(TEXT("%d"), static_cast<int32>(InMaxValue));
	CurrentValue->SetText(FText::FromString(CurrentValueString));
	MaxValue->SetText(FText::FromString(MaxValueString));

	if (IsValid(ProgressBar)) [[likely]]
	{
		ProgressBar->SetPercent(InCurrentValue / InMaxValue);
	}
}
