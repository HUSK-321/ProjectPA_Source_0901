#include "EnvironmentStatusWidget.h"
#include "Components/TextBlock.h"
#include "ProjectPA/Generals/PATypes.h"

void UEnvironmentStatusWidget::OnGameTimeChanged(const FGameTimeInfo& GameTimeInfo)
{
	if (TimeTextBlock)
	{
		TimeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), GameTimeInfo.Hour, GameTimeInfo.Min)));
	}

	if (YearMonthTextBlock)
	{
		YearMonthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%04d.%02d.%02d"), GameTimeInfo.Year, GameTimeInfo.Month, GameTimeInfo.Day)));
	}
}

void UEnvironmentStatusWidget::OnTemperatureChanged(const float NewTemperature)
{
	if (TemperatureTextBlock)
	{
		TemperatureTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), NewTemperature)));
	}
}
