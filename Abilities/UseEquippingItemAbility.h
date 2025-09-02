// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "PAGameplayAbility.h"
#include "ProjectPA/Items/ItemBase.h"
#include "UseEquippingItemAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UUseEquippingItemAbility : public UPAGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item Ability")
	TMap<EItemType, TSubclassOf<UPAGameplayAbility>> ItemTypeAbilityTags;

public:
	UUseEquippingItemAbility();
	
	//~Begin GameplayAbility Functions
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~End GameplayAbility Functions
};
