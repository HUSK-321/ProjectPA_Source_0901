// All copyrights for this code are owned by Aster.


#include "ConsumeDirectlyItemAbility.h"

#include "AbilityObjects/AbilityItemPayloads.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"
#include "ProjectPA/Interfaces/ItemContainer.h"
#include "ProjectPA/Network/UIDContainer.h"

bool UConsumeDirectlyItemAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return UGameplayAbility::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);;
}

void UConsumeDirectlyItemAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (TriggerEventData != nullptr)
	{
		if (const UAbilityItemPayloads* ItemPayload = Cast<UAbilityItemPayloads>(TriggerEventData->OptionalObject))
		{
			ItemInfo = ItemPayload->ItemInfo;
			OwnerUID = ItemPayload->OwnerUID;
		}
	}
#if WITH_EDITORONLY_DATA
	else
	{
		PALog_E(TEXT("Optional Object is not valid..."));
	}
#endif

	if (!ItemInfo.IsValid())
	{
		PALog_E(TEXT("Payload가 유효하지 않거나 ItemInfo 캐싱에 실패했습니다."));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true); 
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UConsumeDirectlyItemAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ItemInfo = FItemEncodedInfo();
	OwnerUID = INDEX_NONE;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

const FItemEncodedInfo* UConsumeDirectlyItemAbility::GetTargetItemInfo() const
{
	return &ItemInfo;
}

FItemEncodedInfo* UConsumeDirectlyItemAbility::GetTargetItemInfo()
{
	TScriptInterface<IPAActor> ItemOwnerActor = GetItemOwnerActor();
	if (ItemOwnerActor == nullptr)
	{
		return nullptr;
	}
	
	if (ItemOwnerActor->GetContainingType() == EUIDType::Character)
	{
		if (AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(ItemOwnerActor.GetObject()))
		{
			return OwnerCharacter->GetItem(ItemInfo);
		}
	}
	else
	{
		if (IItemContainer* ItemContainer = Cast<IItemContainer>(ItemOwnerActor.GetObject()))
		{
			return ItemContainer->GetItem(ItemInfo);
		}
	}

	return nullptr;
}

const FItemDataTable* UConsumeDirectlyItemAbility::GetTargetItemData() const
{
	const IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>();
	check(DataProvider);
	const FItemDataTable* ItemData = DataProvider ? DataProvider->GetItemDataByKey(ItemInfo.GetItemKey()) : nullptr;
	return ItemData;
}

void UConsumeDirectlyItemAbility::DestroyTargetItem()
{
	TScriptInterface<IPAActor> ItemOwnerActor = GetItemOwnerActor();
	if (ItemOwnerActor == nullptr)
	{
		PALog_E(TEXT("ItemOwnerActor is not valid..."));
		return;
	}
	
	if (ItemOwnerActor->GetContainingType() == EUIDType::Character)
	{
		if (AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(ItemOwnerActor.GetObject()))
		{
			OwnerCharacter->RemoveItem(ItemInfo);
		}
	}
	else
	{
		if (IItemContainer* ItemContainer = Cast<IItemContainer>(ItemOwnerActor.GetObject()))
		{
			ItemContainer->RemoveItem(ItemInfo);
		}
	}
}

TScriptInterface<IPAActor> UConsumeDirectlyItemAbility::GetItemOwnerActor()
{
	UUIDContainer* UIDContainer = UUIDContainer::GetInstance(this);
	check(UIDContainer);
	return UIDContainer->GetObjectByUID(OwnerUID);
}
