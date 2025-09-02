// All copyrights for this code are owned by Aster.


#include "DirectUseItemAbility.h"
#include "AbilitySystemComponent.h"

#include "AbilityObjects/AbilityItemPayloads.h"
#include "AbilityObjects/ScopedAbilityEnd.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"
#include "ProjectPA/Items/ItemDataTables.h"

UDirectUseItemAbility::UDirectUseItemAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UDirectUseItemAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{	
	const UAbilityItemPayloads* Payload = TriggerEventData ? Cast<UAbilityItemPayloads>(TriggerEventData->OptionalObject.Get()) : nullptr;
	if (Payload == nullptr || Payload->ItemInfo.IsValid() == false)
	{
		PALog_E(TEXT("PayLoad is nullptr"));
		return;
	}

	const IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>();
	check(DataProvider);
	const FItemDataTable* ItemData = DataProvider ? DataProvider->GetItemDataByKey(Payload->ItemInfo.GetItemKey()) : nullptr;
	if (ItemData == nullptr)
	{
		PALog_E(TEXT("ItemKey '%s' is invalid"), *Payload->ItemInfo.GetItemKey().ToString());
		return;
	}

	const TSubclassOf<UGameplayAbility>* FoundAbilityClass = ItemTypeAbilityMap.Find(ItemData->ItemType);
	if (FoundAbilityClass == nullptr)
	{
		PALog_E(TEXT("ItemType '%d' has no mapped ability class."), ItemData->ItemType);
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		if (FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(*FoundAbilityClass))
		{
			if (AbilitySpec->IsActive() == false)
			{
				ASC->TriggerAbilityFromGameplayEvent(AbilitySpec->Handle, const_cast<FGameplayAbilityActorInfo*>(ActorInfo), FGameplayTag(), TriggerEventData, *ASC);
			}
		}
	}
}
