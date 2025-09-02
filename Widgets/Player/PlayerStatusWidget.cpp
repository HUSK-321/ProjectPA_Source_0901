#include "PlayerStatusWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "ProjectPA/Generals/PATypes.h"
#include "ProjectPA/Widgets/Commons/ProgressBarWithScaleWidget.h"

void UPlayerStatusWidget::SetHealthBar(float MaxHealth, float CurrentHealth)
{
	if (HealthBar && MaxHealth > 0.f)
	{
		HealthBar->SetPercent(CurrentHealth/MaxHealth);
	}
}

void UPlayerStatusWidget::SetStaminaBar(float MaxStamina, float CurrentStamina)
{
	if (StaminaBar && MaxStamina > 0.f)
	{
		StaminaBar->SetPercent(CurrentStamina/MaxStamina);
	}
}

void UPlayerStatusWidget::ToggleBodyPartWidget()
{
	if (IsValid(PartStatusWidget))
	{
		const bool bVisible = PartStatusWidget->IsVisible();
		PartStatusWidget->SetVisibility(bVisible ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);
	}
}

void UPlayerStatusWidget::SetPartValue(const EBodyPart InBodyPart, const float InCurrentValue, const float InMaxValue)
{
	const bool bIsAllFine = IsValid(HeadPart) && IsValid(BodyPart) && IsValid(LeftArmPart) && IsValid(RightArmPart) && IsValid(LeftLegPart) && IsValid(RightLegPart);
	if (bIsAllFine == false) [[unlikely]]
	{
		return;
	}
	
	switch (InBodyPart)
	{
	case EBodyPart::Head: HeadPart->SetValue(InCurrentValue, InMaxValue); break;
	case EBodyPart::Body: BodyPart->SetValue(InCurrentValue, InMaxValue); break;
	case EBodyPart::LeftArm: LeftArmPart->SetValue(InCurrentValue, InMaxValue); break;
	case EBodyPart::RightArm: RightArmPart->SetValue(InCurrentValue, InMaxValue); break;
	case EBodyPart::LeftLeg: LeftLegPart->SetValue(InCurrentValue, InMaxValue); break;
	case EBodyPart::RightLeg: RightLegPart->SetValue(InCurrentValue, InMaxValue); break;
	default: UE_LOG(LogTemp, Error, TEXT("Invalid EBodyPart value received in SetPartValue.")); break;
	}
}
