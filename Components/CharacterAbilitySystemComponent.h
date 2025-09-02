#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.generated.h"

class UPAAbilitySet;

UCLASS()
class PROJECTPA_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSoftObjectPtr<UPAAbilitySet> DefaultAbilitySet;

public:
	//~Begin UActorComponent Functions
	virtual void OnRegister() override;
	//~End UActorComponent Functions

	//~Begin UAbilitySystemComponent Functions
	virtual int32 HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) override;
	virtual void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override { }
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override { }
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override { }
	//~End UAbilitySystemComponent Functions

	void ActivateAbilityWithTag(const FGameplayTag& InputTag, const FGameplayEventData* Payload);

private:
	void GiveDefaultAbilitySet();
};
