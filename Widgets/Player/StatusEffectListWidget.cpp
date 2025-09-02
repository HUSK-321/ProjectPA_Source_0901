// All copyrights for this code are owned by Aster.

#include "StatusEffectListWidget.h"
#include "StatusEffectListElement.h"
#include "Components/WrapBox.h"
#include "ProjectPA/GameplayEffects/StatusEffects/StatusGameplayEffect.h"

void UStatusEffectListWidget::OnStatusEffectListChanged(const TArray<const UStatusGameplayEffect*>& NewList)
{
	if(IsValid(EffectListBox) == false)
	{
		return;
	}

	// TODO : 최적화
	EffectListBox->ClearChildren();
	for (const UStatusGameplayEffect* Effect : NewList)
	{
		UStatusEffectListElement* Element = MakeElement(Effect);
		if (IsValid(Element))
		{
			EffectListBox->AddChildToWrapBox(Element);
		}
	}
}

UStatusEffectListElement* UStatusEffectListWidget::MakeElement(const UStatusGameplayEffect* BaseEffect)
{
	UStatusEffectListElement* NewElement = CreateWidget<UStatusEffectListElement>(GetOwningPlayer(), ListElementClass);
	if (IsValid(NewElement))
	{
		NewElement->SetElement(BaseEffect);
	}
	return NewElement;
}
