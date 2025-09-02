// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ConsumeItemAbilityBase.h"
#include "ConsumeEquipmentItemAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UConsumeEquipmentItemAbility : public UConsumeItemAbilityBase
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
