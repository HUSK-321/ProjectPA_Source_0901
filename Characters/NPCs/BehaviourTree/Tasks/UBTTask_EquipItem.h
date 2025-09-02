// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ProjectPA/Abilities/AbilityObjects/ItemComparisonRules.h"
#include "UBTTask_EquipItem.generated.h"

class UItemComparisonRuleBase;

/**
 * 
 */
UCLASS()
class PROJECTPA_API UUBTTask_EquipItem : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UUBTTask_EquipItem();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

#if WITH_EDITORONLY_DATA
	virtual FString GetStaticDescription() const override;
#endif

protected:
	/** 어빌리티를 활성화할 때 보낼 게임플레이 이벤트 태그입니다. */
	UPROPERTY(EditAnywhere, Category = "GameplayEvent")
	FGameplayTag EventTag;

	/** 장착을 시도할 아이템의 타입입니다. */
	UPROPERTY(EditAnywhere, Category = "Item Query")
	EItemType ItemTypeToEquip;

	/** 아이템을 결정하는 규칙입니다. */
	UPROPERTY(EditAnywhere, Category = "Item Query")
	EItemComparisonLogic ComparisonLogic;
};
