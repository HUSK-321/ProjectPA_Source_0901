// All copyrights for this code are owned by Aster.


#include "EquipItemByQueryAbility.h"

#include "AbilityObjects/AbilityItemPayloads.h"
#include "AbilityObjects/ScopedAbilityEnd.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Components/EquipmentComponent.h"

UEquipItemByQueryAbility::UEquipItemByQueryAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UEquipItemByQueryAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	// Auto-Call EndAbility
	SCOPE_END_ABILITY();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (K2_HasAuthority() == false)
	{
		return;
	}
	
	const UEquipItemQueryPayload* Payload = TriggerEventData ? Cast<UEquipItemQueryPayload>(TriggerEventData->OptionalObject) : nullptr;
	if (Payload == nullptr)
	{
		PALog_W(TEXT("ItemQueryPayload Not Valid"));
		return;
	}

	AGameCharacterBase* Character = Cast<AGameCharacterBase>(ActorInfo->AvatarActor.Get());
	UEquipmentComponent* EquipmentComponent = Character ? Character->GetEquipmentComponent() : nullptr;

	if (Character == nullptr || EquipmentComponent == nullptr)
	{
		return;
	}

	if (const FItemEncodedInfo* BestItemInfo = Character->FindBestItem(Payload->ItemType, Payload->ComparisonLogic))
	{
		if (BestItemInfo->IsValid() && EquipmentComponent->CanEquipItem(*BestItemInfo))
		{
			EquipmentComponent->EquipItem(*BestItemInfo, Character->GetUID());
		}
	}
}
