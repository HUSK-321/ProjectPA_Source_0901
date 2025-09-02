// All copyrights for this code are owned by Aster.


#include "StatusGameplayEffect.h"

#include "AbilitySystemComponent.h"

UTexture2D* UStatusGameplayEffect::GetIcon() const
{
	return StatusEffectIcon.IsNull() == false ? StatusEffectIcon.LoadSynchronous() : nullptr;
}

void UStatusGameplayEffect::ApplyPeriodicEffects(UAbilitySystemComponent* AbilitySystemComponent) const
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	for (const FEffectInfo& Effect : PeriodicEffects)
	{
		if (Effect.Attribute.IsValid())
		{
			// 현재 어트리뷰트 값 가져오기
			float CurrentValue = AbilitySystemComponent->GetNumericAttribute(Effect.Attribute);
			// 새로운 값 적용
			AbilitySystemComponent->SetNumericAttributeBase(Effect.Attribute, CurrentValue + Effect.Amount);
		}
	}
}
