// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDe_HasLineOfSightToTarget.generated.h"

/**
 * AI의 현재 Pawn과 블랙보드에 설정된 TargetActor 사이에 시야(Line of Sight)가 있는지 확인하는 데코레이터
 */
UCLASS()
class PROJECTPA_API UBTDe_HasLineOfSightToTarget : public UBTDecorator
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
private:
	UPROPERTY(EditAnywhere)
	bool bDrawDebug;
#endif

public:
	UBTDe_HasLineOfSightToTarget();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName TargetActorKeyName;

private:
	bool PerformLineTrace(const FVector& StartLoc, const FVector& EndLoc, const AActor* InstigatorActor, const AActor* TargetActor) const;
};
