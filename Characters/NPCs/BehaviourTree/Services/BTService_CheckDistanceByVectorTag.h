// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckDistanceByVectorTag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UBTService_CheckDistanceByVectorTag : public UBTService
{
	GENERATED_BODY()

protected:
	//~Begin UBTService interfaces
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//~End UBTService interfaces
	
private:
	/** 탐색 대상이 될 BlackBoard의 bool 변수 Key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName TargetVectorKey;
	/** 해당 Service 노드의 결과를 저장할 BlackBoard의 bool 변수 Key */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName OutputBoolKey;
	/** 검사할 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TargetDistance;	
};
