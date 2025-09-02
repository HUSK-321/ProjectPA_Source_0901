#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckDistanceByObjectTag.generated.h"

UCLASS()
class PROJECTPA_API UBTService_CheckDistanceByObjectTag : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckDistanceByObjectTag();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	/** 탐색 대상이 될 BlackBoard의 bool 변수 Key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName TargetObjectKey;
	/** 해당 Service 노드의 결과를 저장할 BlackBoard의 bool 변수 Key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName OutputBoolKey;
	/** 검사할 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TargetDistance;	
};
