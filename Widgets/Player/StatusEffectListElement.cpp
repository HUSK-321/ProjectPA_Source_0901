// All copyrights for this code are owned by Aster.


#include "StatusEffectListElement.h"
#include "Components/Image.h"
#include "ProjectPA/GameplayEffects/StatusEffects/StatusGameplayEffect.h"

void UStatusEffectListElement::SetElement(const UStatusGameplayEffect* InStatusEffect)
{
	if (IsValid(StatusIcon))
	{
		StatusIcon->SetBrushFromTexture(InStatusEffect->GetIcon());
	}
}
