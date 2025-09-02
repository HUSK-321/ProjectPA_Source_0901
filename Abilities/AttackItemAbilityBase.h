// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UseItemAbilityBase.h"
#include "ProjectPA/Items/ItemDataTables.h"
#include "AttackItemAbilityBase.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS(Abstract)
class PROJECTPA_API UAttackItemAbilityBase : public UUseItemAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> AttackGE;
	
public:
	UAttackItemAbilityBase();

protected:
	//~Begin UUseEquipmentAbilityBase Functions
	virtual void ActualUseItem() override;
	//~End UUseEquipmentAbilityBase Functions

private:
	void ApplyGameplayEventToTargets(const TArray<FHitResult>& HitResults, const FWeaponItemDataTable* WeaponItem);

	virtual void FinishAbility() override;
	
	static FGameplayAttribute GetRandomDamagePart(bool bIsUpper);
};
