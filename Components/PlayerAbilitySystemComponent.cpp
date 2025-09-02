// All copyrights for this code are owned by Aster.


#include "PlayerAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Abilities/PAGameplayAbility.h"
#include "ProjectPA/Characters/CharacterAttributes/PlayerAttributeSet.h"
#include "ProjectPA/Characters/CharacterAttributes/AttributeObserveSystem/AttributeObserver.h"
#include "ProjectPA/GameplayEffects/StatusEffects/StatusGameplayEffect.h"
#include "ProjectPA/GameStates/PlayGameState.h"

UPlayerAbilitySystemComponent::UPlayerAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UPlayerAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerAbilitySystemComponent, ActiveStatusEffects);
}

bool UPlayerAbilitySystemComponent::GetShouldTick() const
{
	return true;
	// TODO : Find Super Implementation
	//return Super::GetShouldTick();
}

void UPlayerAbilitySystemComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessAbilityInput(DeltaTime);
}

void UPlayerAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UPlayerAbilitySystemComponent::OnActiveGameplayEffectAdded);
	OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UPlayerAbilitySystemComponent::OnRemoveActiveGameplayEffect);

	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UPlayerAbilitySystemComponent::PostBeginPlay);
	}
	
	if (APlayGameState* GameState = GetWorld()->GetGameState<APlayGameState>())
	{
		GameState->OnGameTimeChanged.AddUObject(this, &UPlayerAbilitySystemComponent::OnGameTimeChanged);
	}

	if (IsValid(AttributeObserveClass) && IsOwnerActorAuthoritative())
	{
		AttributeObserver = NewObject<UAttributeObserver>(this, AttributeObserveClass);
		if (IsValid(AttributeObserver))
		{
			AttributeObserver->InitializeObserver(Cast<UPlayerAttributeSet>(GetAttributeSet(UPlayerAttributeSet::StaticClass())));
		}
	}
}

void UPlayerAbilitySystemComponent::PostBeginPlay()
{
	OnStatusEffectsUpdated.Broadcast(ActiveStatusEffects);
}

void UPlayerAbilitySystemComponent::OnActiveGameplayEffectAdded(UAbilitySystemComponent* ASC,
                                                                const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	if (const UStatusGameplayEffect* StatusEffect = Cast<UStatusGameplayEffect>(Spec.Def))
	{
		AddStatusEffect(StatusEffect);
	}
}

void UPlayerAbilitySystemComponent::OnRemoveActiveGameplayEffect(const FActiveGameplayEffect& Effect)
{
	if (const UStatusGameplayEffect* StatusEffect = Cast<UStatusGameplayEffect>(Effect.Spec.Def))
	{
		RemoveStatusEffect(StatusEffect);
	}
}

void UPlayerAbilitySystemComponent::AddStatusEffect(const UStatusGameplayEffect* StatusEffect)
{
	if (IsValid(StatusEffect) == false)
	{
		return;
	}

	// 만일 같은 카테고리의 낮은 레벨의 상태이상이 있었다고 하면 제거한 뒤 새 이펙트를 넣습니다. 
	if (const UStatusGameplayEffect* AlreadyHaveStatus = GetStatusEffectByCategory(StatusEffect->GetStatusCategory()))
	{
		if (AlreadyHaveStatus->GetEffectLevel() >= StatusEffect->GetEffectLevel())
		{
			return;
		}
		RemoveStatusEffect(AlreadyHaveStatus, false);
	}
	
	ActiveStatusEffects.Add(StatusEffect);
	OnRep_ActiveStatusEffects();
}

void UPlayerAbilitySystemComponent::RemoveStatusEffect(const UStatusGameplayEffect* StatusEffect, bool bBroadcastEvent)
{
	if (IsValid(StatusEffect) == false)
	{
		return;
	}
	
	ActiveStatusEffects.Remove(StatusEffect);

	if (bBroadcastEvent)
	{
		OnRep_ActiveStatusEffects();
		OnStatusEffectsUpdated.Broadcast(ActiveStatusEffects);
	}
}

const UStatusGameplayEffect* UPlayerAbilitySystemComponent::GetStatusEffectByCategory(const FGameplayTag& EffectTag)
{
	for (const UStatusGameplayEffect* StatusEffect : ActiveStatusEffects)
	{
		if (StatusEffect->GetStatusCategory() == EffectTag)
		{
			return StatusEffect;
		}
	}
	return nullptr;
}

void UPlayerAbilitySystemComponent::OnRep_ActiveStatusEffects()
{
	OnStatusEffectsUpdated.Broadcast(ActiveStatusEffects);
}

void UPlayerAbilitySystemComponent::OnGameTimeChanged(const FGameTimeInfo& NewGameTime)
{
	if (IsOwnerActorAuthoritative() == false)
	{
		return;
	}

	for (const UStatusGameplayEffect* StatusEffect : ActiveStatusEffects)
	{
		if (IsValid(StatusEffect))
		{
			StatusEffect->ApplyPeriodicEffects(this);
		}
	}
}

void UPlayerAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UPlayerAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UPlayerAbilitySystemComponent::ProcessAbilityInput(float DeltaTime)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();


	// Process all abilities that activate when the input is held.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UPAGameplayAbility* LyraAbilityCDO = Cast<UPAGameplayAbility>(AbilitySpec->Ability);
				if (LyraAbilityCDO && LyraAbilityCDO->GetActivationPolicy() == EAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}
	
	// Process all abilities that had their input pressed this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UPAGameplayAbility* LyraAbilityCDO = Cast<UPAGameplayAbility>(AbilitySpec->Ability);

					if (LyraAbilityCDO && LyraAbilityCDO->GetActivationPolicy() == EAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}
	
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}
	
	// Process all abilities that had their input released this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}
	
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UPlayerAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}
