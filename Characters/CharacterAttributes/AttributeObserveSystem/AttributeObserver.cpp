// All copyrights for this code are owned by Aster.

#include "AttributeObserver.h"
#include "ProjectPA/Characters/CharacterAttributes/PlayerAttributeSet.h"

void UAttributeObserver::InitializeObserver(const UPlayerAttributeSet* PlayerAttributeSet)
{
	if (IsValid(PlayerAttributeSet) == false)
	{
		return;
	}

	PlayerAttributeSet->AttributeChangedEvent.AddUObject(this, &UAttributeObserver::OnAttributeChanged);

	// Record Prev Status
	TargetASC = PlayerAttributeSet->GetOwningAbilitySystemComponent();
	if (IsValid(TargetASC))
	{
		for (const auto& AttributeToObserve : AttributesToObserve)
		{
			PrevAttributeStatuses.Emplace(AttributeToObserve.Key, {TargetASC->GetNumericAttribute(AttributeToObserve.Key), false} );
		}
	}
}

void UAttributeObserver::OnAttributeChanged(const FGameplayAttribute& ChangedAttribute)
{
	if (AttributesToObserve.Find(ChangedAttribute) == nullptr || AttributesToObserve[ChangedAttribute].InfoList.IsEmpty())
	{
		return;
	}

	const float CurAttributeValue = TargetASC->GetNumericAttribute(ChangedAttribute);
	// TODO : Cannot crash? (while not applied)
	if (PrevAttributeStatuses[ChangedAttribute].Value == CurAttributeValue)
	{
		return;
	}

	TArray<FObserveInfo>& ObserveInfos = AttributesToObserve[ChangedAttribute].InfoList;
	for (FObserveInfo& ObserveInfo : ObserveInfos)
	{
		const bool bShouldApplyEvent = CheckAttributeThreshold(ObserveInfo, CurAttributeValue);
		if (bShouldApplyEvent != PrevAttributeStatuses[ChangedAttribute].bEventApplied)
		{
			bShouldApplyEvent ? ApplyEffects(ObserveInfo) :
								RemoveEffects(ObserveInfo);
			PrevAttributeStatuses[ChangedAttribute].bEventApplied = bShouldApplyEvent;
		}	
	}

	PrevAttributeStatuses[ChangedAttribute].Value = CurAttributeValue;
}

void UAttributeObserver::CalculateAllEffects()
{
	for (auto& AttributeStatus : PrevAttributeStatuses)
	{
		const auto& Attribute = AttributeStatus.Key;
		if (AttributesToObserve.Find(Attribute) == nullptr || AttributesToObserve[Attribute].InfoList.IsEmpty())
		{
			continue;
		}

		const float PrevValue = PrevAttributeStatuses[Attribute].Value;
		TArray<FObserveInfo>& ObserveInfos = AttributesToObserve[Attribute].InfoList;
		for (FObserveInfo& ObserveInfo : ObserveInfos)
		{
			const bool bShouldApplyEvent = CheckAttributeThreshold(ObserveInfo, PrevValue);
			if (bShouldApplyEvent != PrevAttributeStatuses[Attribute].bEventApplied)
			{
				bShouldApplyEvent ? ApplyEffects(ObserveInfo) :
									RemoveEffects(ObserveInfo);
				PrevAttributeStatuses[Attribute].bEventApplied = bShouldApplyEvent;
			}
		}
	}
}

bool UAttributeObserver::CheckAttributeThreshold(const FObserveInfo& AttributeToObserve, const float CurrentValue)
{
	switch (AttributeToObserve.Calculator)
	{
	case ObserveCalculator::LessThan: // 기준값 이하 (<=)
		return CurrentValue <= AttributeToObserve.Threshold;
	case ObserveCalculator::MoreThan: // 기준값 이상 (>=)
		return CurrentValue >= AttributeToObserve.Threshold;
	case ObserveCalculator::Under: // 기준값 미만 (<)
		return CurrentValue < AttributeToObserve.Threshold;
	case ObserveCalculator::Over: // 기준값 초과 (>)
		return CurrentValue > AttributeToObserve.Threshold;
	case ObserveCalculator::Equal:
		return CurrentValue == AttributeToObserve.Threshold;
	case ObserveCalculator::NotEqual:
		return CurrentValue != AttributeToObserve.Threshold;
	default:
		break;
	}

	return false;
}

void UAttributeObserver::ApplyEffects(FObserveInfo& AttributeToObserve)
{
	if (IsValid(TargetASC) == false || AttributeToObserve.EffectsToApply.Num() == 0)
	{
		return;
	}

	for (const TSubclassOf<UGameplayEffect>& EffectClass : AttributeToObserve.EffectsToApply)
	{
		if (!EffectClass)
		{
			continue;
		}

		FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(EffectClass, 1.0f, TargetASC->MakeEffectContext());
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			AttributeToObserve.AppliedEffectHandles.Emplace(ActiveGameplayEffectHandle);
		}
	}
}

void UAttributeObserver::RemoveEffects(FObserveInfo& AttributeToObserve)
{
	if (IsValid(TargetASC) == false || AttributeToObserve.AppliedEffectHandles.Num() == 0)
	{
		return;
	}

	for (const FActiveGameplayEffectHandle& Handle : AttributeToObserve.AppliedEffectHandles)
	{
		if (Handle.IsValid())
		{
			TargetASC->RemoveActiveGameplayEffect(Handle);
		}
	}
    
	AttributeToObserve.AppliedEffectHandles.Empty();
}
