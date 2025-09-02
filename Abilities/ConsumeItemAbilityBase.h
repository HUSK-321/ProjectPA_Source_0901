// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UseItemAbilityBase.h"
#include "ConsumeItemAbilityBase.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS(Abstract)
class PROJECTPA_API UConsumeItemAbilityBase : public UUseItemAbilityBase
{
	GENERATED_BODY()

private:
	/** ~~% 이상 몽타주 재생 시 아이템 사용을 다 했다고 가정할 것 */
	UPROPERTY(EditAnywhere, Category = "Consume Item")
	float MontageCompletionPercentage;

	float CachedMontagePlayTime;
	float CachedMontageStartTime;

public:
	UConsumeItemAbilityBase();
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	//~Begin UUseEquipmentAbilityBase Functions
	virtual void ActualUseItem() override;
	//~End UUseEquipmentAbilityBase Functions

private:
	void ConsumeItem(const float UseRatio);
};
