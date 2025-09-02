// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "PAGameplayAbility.h"
#include "EquipItemByQueryAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UEquipItemByQueryAbility : public UPAGameplayAbility
{
	GENERATED_BODY()

public:
	UEquipItemByQueryAbility();

	//~Begin UGameplayAbility interfaces
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~End UGameplayAbility interfaces
};
