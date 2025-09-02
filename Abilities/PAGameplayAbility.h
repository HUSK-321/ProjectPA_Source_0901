#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PAGameplayAbility.generated.h"

UENUM()
enum class EAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn
};

UCLASS()
class PROJECTPA_API UPAGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EAbilityActivationPolicy AbilityActivationPolicy;

public:
	UPAGameplayAbility();

	/** 외부에서 EndAbility 를 호출하기 위한 인터페이스입니다. */
	void SafeEndAbility() { K2_EndAbility(); }
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	FORCEINLINE EAbilityActivationPolicy GetActivationPolicy() const { return AbilityActivationPolicy; }
protected:
	virtual void TryActivatePassiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);
	
	template < class T >
	T* GetSourceObject() const
	{
		if (const FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec())
		{
			return Cast<T>(Spec->SourceObject.Get());
		}
		return nullptr;
	}
};
