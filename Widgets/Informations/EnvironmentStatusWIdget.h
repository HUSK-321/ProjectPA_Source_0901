#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "EnvironmentStatusWidget.generated.h"

struct FGameTimeInfo;
class UTextBlock;

/**
 * @brief 시간 날씨 등을 알려주는 위젯
 */
UCLASS()
class PROJECTPA_API UEnvironmentStatusWidget : public UPAWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> YearMonthTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TimeTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TemperatureTextBlock;

public:
	void OnGameTimeChanged(const FGameTimeInfo& GameTimeInfo);
	void OnTemperatureChanged(const float NewTemperature);
};
