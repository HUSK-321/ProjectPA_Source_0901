#include "PAGameplayAbility.h"
#include "AbilitySystemComponent.h"

UPAGameplayAbility::UPAGameplayAbility()
	: AbilityActivationPolicy(EAbilityActivationPolicy::OnInputTriggered)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

void UPAGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (GetActivationPolicy() == EAbilityActivationPolicy::OnSpawn)
	{
		TryActivatePassiveAbility(ActorInfo, Spec);
	}
}

void UPAGameplayAbility::TryActivatePassiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	const bool bIsPredicting = (CurrentActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);
 
	// Try to activate if activation type is Passive. Passive abilities are auto activated when given.
	if (ActorInfo && Spec.IsActive() == false && bIsPredicting == false)
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();
 
		// If avatar actor is torn off or about to die, don't try to activate it.
		if (ASC && AvatarActor && AvatarActor->GetTearOff() == false && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);
 
			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;
 
			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}
