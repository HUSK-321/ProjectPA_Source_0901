// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAbilitySystemComponent.h"
#include "ProjectPA/Generals/PATypes.h"
#include "PlayerAbilitySystemComponent.generated.h"

class UAttributeObserver;
class UStatusGameplayEffect;

DECLARE_EVENT_OneParam(UPlayerAbilitySystemComponent, FOnStatusEffectListUpdated, const TArray<const UStatusGameplayEffect*>& /** Effects */)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTPA_API UPlayerAbilitySystemComponent : public UCharacterAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/** 상태이상 업데이트 델리게이트 */
	FOnStatusEffectListUpdated OnStatusEffectsUpdated;
	
private:
	/** 현재 적용된 상태이상 목록 */
	UPROPERTY(ReplicatedUsing = OnRep_ActiveStatusEffects)
	TArray<TObjectPtr<const UStatusGameplayEffect>> ActiveStatusEffects;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeObserver> AttributeObserveClass;
	UPROPERTY()
	TObjectPtr<UAttributeObserver> AttributeObserver;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
public:
	UPlayerAbilitySystemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool GetShouldTick() const override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime);
	void ClearAbilityInput();

protected:
	virtual void BeginPlay() override;
	void PostBeginPlay();

protected:
	void OnActiveGameplayEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	void OnRemoveActiveGameplayEffect(const FActiveGameplayEffect& Effect);

private:
	void AddStatusEffect(const UStatusGameplayEffect* StatusEffect);
	void RemoveStatusEffect(const UStatusGameplayEffect* StatusEffect, bool bBroadcastEvent = true);

	const UStatusGameplayEffect* GetStatusEffectByCategory(const FGameplayTag& EffectTag);

	UFUNCTION()
	void OnRep_ActiveStatusEffects();

	void OnGameTimeChanged(const FGameTimeInfo& NewGameTime);
};
