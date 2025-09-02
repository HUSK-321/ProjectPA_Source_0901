// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ConsumeItemAbilityBase.h"
#include "ProjectPA/Generals/PATypes.h"
#include "ConsumeDirectlyItemAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UConsumeDirectlyItemAbility : public UConsumeItemAbilityBase
{
	GENERATED_BODY()

private:
	FItemEncodedInfo ItemInfo;
	int32 OwnerUID = INDEX_NONE;

protected:
	//~Begin UUseEquipmentAbilityBase Functions
	/** Equipment 가 아니라 CurrentEventData 를 통해 아이템 정보를 받는 함수들의 경우
	 *	PreActivate 에서 CurrentEventData 가 세팅되므로 CanActivateAbility 에서는 return true 가 되도록 함
	 */
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual const FItemEncodedInfo* GetTargetItemInfo() const override;
	virtual FItemEncodedInfo* GetTargetItemInfo() override;
	virtual const FItemDataTable* GetTargetItemData() const override;
	
	virtual void DestroyTargetItem() override;
	//~End UUseEquipmentAbilityBase Functions

private:
	TScriptInterface<IPAActor> GetItemOwnerActor();
};
