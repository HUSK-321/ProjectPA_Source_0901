// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "PAGameplayAbility.h"
#include "UseItemAbilityBase.generated.h"

struct FItemEncodedInfo;
struct FItemDataTable;
enum class EItemType : uint8;
class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

UCLASS(Abstract)
class PROJECTPA_API UUseItemAbilityBase : public UPAGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = Item)
	FGameplayTag ItemUseTag;
	UPROPERTY(EditDefaultsOnly, Category = Item)
	EItemType AbilityItemType;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEventTask;
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

protected:
	UUseItemAbilityBase();
	
	//~Begin GameplayAbility Functions
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End GameplayAbility Functions

protected:
	// Get TargetItem Functions (Use These Functions to get Item)

	virtual const FItemEncodedInfo* GetTargetItemInfo() const { return nullptr; }
	virtual FItemEncodedInfo* GetTargetItemInfo() { return nullptr; }
	virtual const FItemDataTable* GetTargetItemData() const { return nullptr; }
	virtual void ActualUseItem() { }
	virtual void DestroyTargetItem() { }
	
	UFUNCTION()
	virtual void OnMontageFinished();
	UFUNCTION()
	virtual void OnMontageCancelled();
	UFUNCTION()
	void OnGetItemUseTagEvent(FGameplayEventData Payload);

	virtual void FinishAbility();
	
};
