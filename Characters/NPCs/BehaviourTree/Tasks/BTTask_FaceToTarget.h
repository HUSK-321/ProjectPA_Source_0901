#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FaceToTarget.generated.h"

UCLASS()
class PROJECTPA_API UBTTask_FaceToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FaceToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName TargetPlayerKey;
};
