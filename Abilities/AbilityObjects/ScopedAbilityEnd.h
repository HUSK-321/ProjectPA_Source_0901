// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Abilities/PAGameplayAbility.h"

#define SCOPE_END_ABILITY() const FScopedAbilityEnd ANONYMOUS_VARIABLE(ScopedEnd)(this);

/**
 * 아래 구조체는 RAII 패턴으로 만들어 져 해당하는 스코프를 벗어나면 바로 EndAbility 를 호출하게 됩니다.
 * 어빌리티에서만 사용 가능하며 특수한 경우가 아니라면 사용하지 말도록 합니다.
 */
struct FScopedAbilityEnd
{
	explicit FScopedAbilityEnd(UPAGameplayAbility* InAbility, bool bForceCancel = false)
		: Ability(InAbility)
		, bShouldCancel(bForceCancel)
	{
	}

	~FScopedAbilityEnd()
	{
		if (Ability && Ability->IsActive())
		{
			if (bShouldCancel)
			{
				Ability->K2_CancelAbility();
			}
			else
			{
				Ability->SafeEndAbility();
			}
		}
	}

	FScopedAbilityEnd(const FScopedAbilityEnd&) = delete;
	FScopedAbilityEnd& operator=(const FScopedAbilityEnd&) = delete;

private:
	UPAGameplayAbility* Ability = nullptr;
	bool bShouldCancel = false;
};
