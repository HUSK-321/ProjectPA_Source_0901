// All copyrights for this code are owned by Aster.


#include "UseEquippingItemAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilityObjects/ScopedAbilityEnd.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Components/EquipmentComponent.h"
#include "ProjectPA/Items/ItemDataTables.h"

UUseEquippingItemAbility::UUseEquippingItemAbility()
{
}

bool UUseEquippingItemAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                  const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	if (const AActor* OwnerActor = ActorInfo->AvatarActor.Get())
	{
		if (UEquipmentComponent* EquipmentComponent = OwnerActor->FindComponentByClass<UEquipmentComponent>())
		{
			return EquipmentComponent->GetEquippingItem_Ref().IsValid();
		}
	}
    
	return false;
}

void UUseEquippingItemAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// Auto-Call EndAbility
	SCOPE_END_ABILITY();
	
	if (HasAuthority(&ActivationInfo) == false)
	{
		return;
	}

	const AActor* OwnerActor = ActorInfo->AvatarActor.Get();
	UEquipmentComponent* EquipmentComponent = OwnerActor ? OwnerActor->FindComponentByClass<UEquipmentComponent>() : nullptr;
    
	if (IsValid(EquipmentComponent) == false)
	{
		PALog_E(TEXT("EquipmentComponent is nullptr"));
		return;
	}
    
	const FItemEncodedInfo& EquippingSlot = EquipmentComponent->GetEquippingItem();
	if (EquippingSlot.IsValid() == false)
	{
		PALog_E(TEXT("EquippingSlot is not valid"));
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (FItemDataTable* ItemDataTable = EquipmentComponent->GetEquippingItemData())
	{
		EItemType ItemType = ItemDataTable->ItemType;
		if (ItemTypeAbilityTags.Contains(ItemType))
		{
			if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
			{
				ASC->TryActivateAbilityByClass(ItemTypeAbilityTags[ItemType]);
			}
		}	
	}
}
