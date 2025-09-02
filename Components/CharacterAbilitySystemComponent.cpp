#include "CharacterAbilitySystemComponent.h"
#include "ProjectPA/Abilities/PAAbilitySet.h"
#include "ProjectPA/Abilities/PAGameplayAbility.h"

void UCharacterAbilitySystemComponent::OnRegister()
{
	Super::OnRegister();
	
	GiveDefaultAbilitySet();
}

int32 UCharacterAbilitySystemComponent::HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	const FGameplayTag InputTagIdentifier = FGameplayTag::RequestGameplayTag(FName("Input"));
	
	if (EventTag.MatchesTag(InputTagIdentifier))
	{
		ActivateAbilityWithTag(EventTag, Payload);
	}
	
	return Super::HandleGameplayEvent(EventTag, Payload);
}

void UCharacterAbilitySystemComponent::ActivateAbilityWithTag(const FGameplayTag& InputTag, const FGameplayEventData* Payload)
{
	if (InputTag.IsValid() == false)
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			TriggerAbilityFromGameplayEvent(AbilitySpec.Handle, AbilityActorInfo.Get(), InputTag, Payload, *this);
			break;
		}
	}
}

void UCharacterAbilitySystemComponent::GiveDefaultAbilitySet()
{
	if (DefaultAbilitySet.IsValid() == false && IsOwnerActorAuthoritative() == false)
	{
		return;
	}

	if (const UPAAbilitySet* Ability = DefaultAbilitySet.LoadSynchronous())
	{
		Ability->GiveAbilityToTarget(this, this);
	}
}
