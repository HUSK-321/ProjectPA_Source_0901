// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ProjectPA/Generals/PATypes.h"
#include "BTDecorator_CheckEquippingItemType.generated.h"

/**
 * AI 캐릭터가 현재 장착한 아이템이 지정된 타입과 일치하는지 확인하는 데코레이터입니다.
 */
UCLASS()
class PROJECTPA_API UBTDecorator_CheckEquippingItemType : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckEquippingItemType();

protected:
	//~ Begin UBTDecorator Interface
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	//~ End UBTDecorator Interface

#if WITH_EDITORONLY_DATA
	virtual FString GetStaticDescription() const override;
#endif

protected:
	/** 검사할 아이템의 타입입니다. */
	UPROPERTY(EditAnywhere, Category = "Item")
	EItemType ItemTypeToCheck;
};