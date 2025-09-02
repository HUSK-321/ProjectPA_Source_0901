#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "PlayerStatusWidget.generated.h"

class UCanvasPanel;
enum class EBodyPart : uint8;
class UProgressBarWithScaleWidget;
class UProgressBar;

UCLASS()
class PROJECTPA_API UPlayerStatusWidget : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> PartStatusWidget;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBarWithScaleWidget> HeadPart;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBarWithScaleWidget> BodyPart;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBarWithScaleWidget> LeftArmPart;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBarWithScaleWidget> RightArmPart;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBarWithScaleWidget> LeftLegPart;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBarWithScaleWidget> RightLegPart;

public:
	void SetHealthBar(float MaxHealth, float CurrentHealth);
	void SetStaminaBar(float MaxStamina, float CurrentStamina);

	void ToggleBodyPartWidget();
	void SetPartValue(const EBodyPart InBodyPart, const float InCurrentValue, const float InMaxValue = 100.0f);

};
