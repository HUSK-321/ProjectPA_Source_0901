// Fill out your copyright notice in the Description page of Project Settings.

#include "PAAbilitySet.h"
#include "PAGameplayAbility.h"
#include "ProjectPA/Components/CharacterAbilitySystemComponent.h"

void UPAAbilitySet::GiveAbilityToTarget(UCharacterAbilitySystemComponent* ASC, UObject* SourceObject) const
{
	if (IsValid(ASC) == false)
	{
		return;
	}

	for (const FAbilityInfo& AbilityInfo : AbilityInfos)
	{
		if (AbilityInfo.AbilityClass == nullptr)
		{
			continue;
		}

		if (UPAGameplayAbility* AbilityCDO = AbilityInfo.AbilityClass->GetDefaultObject<UPAGameplayAbility>())
		{
			FGameplayAbilitySpec Spec(AbilityCDO, AbilityInfo.AbilityLevel);
			Spec.SourceObject = SourceObject;
			ASC->GiveAbility(Spec);
		}
	}

	for (const FAbilityInfoWithInput& AbilityInfoWithInput : AbilityInfosWithInput)
	{
		if (AbilityInfoWithInput.AbilityClass == nullptr)
		{
			continue;
		}

		if (UPAGameplayAbility* AbilityCDO = AbilityInfoWithInput.AbilityClass->GetDefaultObject<UPAGameplayAbility>())
		{
			FGameplayAbilitySpec Spec(AbilityCDO, AbilityInfoWithInput.AbilityLevel);
			Spec.SourceObject = SourceObject;
			Spec.GetDynamicSpecSourceTags().AddTag(AbilityInfoWithInput.InputTag);
			
			ASC->GiveAbility(Spec);
		}
	}
}
