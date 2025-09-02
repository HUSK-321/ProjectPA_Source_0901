// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "PAGameplayAbility.h"
#include "DirectUseItemAbility.generated.h"

/**
 * 인벤토리 등 외부 컨텍스트에서 아이템 사용을 요청할 때 사용되는 '라우터' 어빌리티입니다.
 * GameplayEvent로 Payload를 받아, 아이템 타입에 맞는 실제 어빌리티를 트리거합니다.
 */
UCLASS()
class PROJECTPA_API UDirectUseItemAbility : public UPAGameplayAbility
{
	GENERATED_BODY()

public:
	UDirectUseItemAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** 아이템 타입에 따라 실행할 구체적인 어빌리티 클래스를 매핑합니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Ability")
	TMap<EItemType, TSubclassOf<UGameplayAbility>> ItemTypeAbilityMap;
};
