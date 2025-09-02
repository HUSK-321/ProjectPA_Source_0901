// All copyrights for this code are owned by Aster.

#include "UEnvQueryContext_GetBlackboardActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

UUEnvQueryContext_GetBlackboardActor::UUEnvQueryContext_GetBlackboardActor(const FObjectInitializer& ObjectInitializer)
{
	BlackboardKeyName = TEXT("TargetActor");
}

void UUEnvQueryContext_GetBlackboardActor::ProvideContext(FEnvQueryInstance& QueryInstance,
	FEnvQueryContextData& ContextData) const
{
	if (QueryInstance.Owner.IsValid() == false)
	{
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(QueryInstance.Owner.Get());
	AAIController* AIController = IsValid(OwnerPawn) ? OwnerPawn->GetController<AAIController>() : Cast<AAIController>(QueryInstance.Owner.Get());
	if (IsValid(AIController))
	{
		if (const UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
		{
			const AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(BlackboardKeyName));
			
			if (TargetActor)
			{
				UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
			}
		}
	}
}
