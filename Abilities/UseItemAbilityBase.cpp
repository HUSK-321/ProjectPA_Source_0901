// All copyrights for this code are owned by Aster.


#include "UseItemAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Components/EquipmentComponent.h"
#include "ProjectPA/Items/ItemDataTables.h"

UUseItemAbilityBase::UUseItemAbilityBase()
	: AbilityItemType(EItemType::MAX)
{
}

bool UUseItemAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                  const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	const FItemEncodedInfo* TargetItemInfo = GetTargetItemInfo();
	const FItemDataTable* EquippingItemData = GetTargetItemData();
	if (EquippingItemData == nullptr || TargetItemInfo == nullptr)
	{
		return false;
	}
	if (EquippingItemData)
	{
		return TargetItemInfo->IsValid() && EquippingItemData->ItemType == AbilityItemType;
	}
    
	return false;
}

void UUseItemAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ItemUseTag);
	if (WaitGameplayEventTask)
	{
		WaitGameplayEventTask->EventReceived.AddDynamic(this, &UUseItemAbilityBase::OnGetItemUseTagEvent);
		WaitGameplayEventTask->Activate();
	}
	
	FItemEncodedInfo* TargetItemInfo = GetTargetItemInfo();
	const FItemDataTable* TargetItemData = GetTargetItemData();
	if (TargetItemData == nullptr || TargetItemInfo == nullptr || TargetItemInfo->IsValid() == false)
	{
		K2_EndAbility();
		return;
	}

	if (TargetItemData->GetMontageToPlay())
	{
		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, TargetItemData->GetMontageToPlay(), 1.0f);
		if (MontageTask)
		{
			MontageTask->OnCompleted.AddDynamic(this, &UUseItemAbilityBase::OnMontageFinished);
			MontageTask->OnCancelled.AddDynamic(this, &UUseItemAbilityBase::OnMontageCancelled);
			MontageTask->OnInterrupted.AddDynamic(this, &UUseItemAbilityBase::OnMontageCancelled);
			MontageTask->OnBlendOut.AddDynamic(this, &UUseItemAbilityBase::OnMontageFinished);
			MontageTask->Activate();
		}
	}
	else
	{
		ActualUseItem();
		K2_EndAbility();
		return;
	}
}

void UUseItemAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUseItemAbilityBase::OnMontageFinished()
{
	FinishAbility();
}

void UUseItemAbilityBase::OnMontageCancelled()
{
	FinishAbility();
}

void UUseItemAbilityBase::OnGetItemUseTagEvent(FGameplayEventData Payload)
{
	ActualUseItem();
}

void UUseItemAbilityBase::FinishAbility()
{
	if (IsValid(WaitGameplayEventTask))
	{
		WaitGameplayEventTask->EndTask();
		WaitGameplayEventTask = nullptr;
	}

	if (IsValid(MontageTask))
	{
		MontageTask->EndTask();
		MontageTask = nullptr;
	}
	
	K2_EndAbility();
}
