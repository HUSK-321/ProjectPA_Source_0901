// All copyrights for this code are owned by Aster.


#include "ConsumeItemAbilityBase.h"
#include "AbilitySystemComponent.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Items/ItemDataTables.h"

UConsumeItemAbilityBase::UConsumeItemAbilityBase()
	: MontageCompletionPercentage(0.85f)
	, CachedMontagePlayTime(0.0f)
	, CachedMontageStartTime(0.0f)
{
}

void UConsumeItemAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (K2_HasAuthority())
	{
		AGameCharacterBase* AbilityOwner = Cast<AGameCharacterBase>(GetActorInfo().AvatarActor);
		if (IsValid(AbilityOwner) == false)
		{
			K2_EndAbility();
			return;
		}
		
		if (const FItemDataTable* ItemData = GetTargetItemData())
		{
			if (UAnimMontage* MontageToPlay = ItemData->GetMontageToPlay())
			{
				CachedMontagePlayTime = MontageToPlay->GetPlayLength();
				CachedMontageStartTime = GetWorld()->GetTimeSeconds();
			}
		}
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UConsumeItemAbilityBase::ActualUseItem()
{
	if (K2_HasAuthority() == false || GetWorld() == nullptr)
	{
		K2_EndAbility();
		return;
	}

	const float TimeDelta = GetWorld()->GetTimeSeconds() - CachedMontageStartTime;
	float PlayRate = TimeDelta / CachedMontagePlayTime;
	MontageCompletionPercentage = 0.85f;

	if (PlayRate > MontageCompletionPercentage)
	{
		PlayRate = 1.0f;
	}

	ConsumeItem(PlayRate);
}

void UConsumeItemAbilityBase::ConsumeItem(const float UseRatio)
{
	UWorld* World = GetWorld();
	if (K2_HasAuthority() == false || World == nullptr)
	{
		K2_EndAbility();
		return;
	}

	// 아이템의 내구도를 사용하기 

	if (AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(GetActorInfo().AvatarActor))
	{
		const FConsumeItemDataTable* ItemData = static_cast<const FConsumeItemDataTable*>(GetTargetItemData());
		UAbilitySystemComponent* ASC = OwnerCharacter->GetAbilitySystemComponent();
		if (ItemData && IsValid(ASC))
		{
			for (const FConsumeEffect& Modifier : ItemData->ConsumeEffects)
			{
				const float CurrentValue = ASC->GetNumericAttribute(Modifier.Attribute);
				const float NewValue = CurrentValue + Modifier.Value;
				ASC->ApplyModToAttribute(Modifier.Attribute, EGameplayModOp::Additive, NewValue);
			}	
		}
	
		FItemEncodedInfo* TargetItemInfo = GetTargetItemInfo();
		if (TargetItemInfo != nullptr && TargetItemInfo->IsValid())
		{
			const int32 NewRemainPercentage = TargetItemInfo->GetItemRemainPercentage() - UseRatio * 100.0f;

			if (NewRemainPercentage > 0)
			{
				CachedMontageStartTime = World->GetTimeSeconds();
				TargetItemInfo->SetItemRemainPercentage(NewRemainPercentage);
			}
			else
			{
				DestroyTargetItem();
				K2_EndAbility();
			}
		}
	}
}
