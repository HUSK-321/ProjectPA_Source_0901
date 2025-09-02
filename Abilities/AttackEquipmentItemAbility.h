// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "AttackItemAbilityBase.h"
#include "AttackEquipmentItemAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UAttackEquipmentItemAbility : public UAttackItemAbilityBase
{
	GENERATED_BODY()

protected:
	//~Begin UUseEquipmentAbilityBase Functions
	virtual const FItemEncodedInfo* GetTargetItemInfo() const override;
	virtual FItemEncodedInfo* GetTargetItemInfo() override;
	virtual FItemDataTable* GetTargetItemData() const override;
	
	virtual void DestroyTargetItem() override;
	//~End UUseEquipmentAbilityBase Functions
};
