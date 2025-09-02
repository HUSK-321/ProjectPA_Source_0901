#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDe_CheckDistanceByObjectTag.generated.h"


UCLASS()
class PROJECTPA_API UBTDe_CheckDistanceByObjectTag : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDe_CheckDistanceByObjectTag();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	/** 탐색 대상이 될 BlackBoard의 Actor 변수 Key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName TargetObjectKey;
	/** 해당 Decorator 노드의 결과를 저장할 BlackBoard의 bool 변수 Key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName OutputBoolKey;
	/** 검사할 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TargetDistance;	
};
