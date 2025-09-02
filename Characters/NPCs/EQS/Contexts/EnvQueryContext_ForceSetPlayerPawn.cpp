// All copyrights for this code are owned by Aster.


#include "EnvQueryContext_ForceSetPlayerPawn.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

UEnvQueryContext_ForceSetPlayerPawn::UEnvQueryContext_ForceSetPlayerPawn()
{
	BlackboardKeyName = TEXT("TargetActor"); 
}

void UEnvQueryContext_ForceSetPlayerPawn::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                         FEnvQueryContextData& ContextData) const
{
	APawn* OwnerPawn = Cast<APawn>(QueryInstance.Owner.Get());
	AAIController* AIController = IsValid(OwnerPawn) ? OwnerPawn->GetController<AAIController>() : Cast<AAIController>(QueryInstance.Owner.Get());

    if (IsValid(AIController) == false)
    {
        return;
    }

    const APawn* QuerierPawn = AIController->GetPawn();
    if (IsValid(QuerierPawn) == false)
    {
        return;
    }

    FDistanceComparePredicate IsBetterDistanceLambda = (SearchType == EPlayerSearchType::Closest)
    ? [](const float CurrentDistance, const float BestDistance) {
            return CurrentDistance < BestDistance;
        }
        : [](const float CurrentDistance, const float BestDistance) {
            return CurrentDistance > BestDistance;
        };
    
    if (APawn* FoundPlayerPawn = FindTargetPlayerPawn(QueryInstance.World, QuerierPawn, IsBetterDistanceLambda))
    {
        if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
        {
            BlackboardComp->SetValueAsObject(BlackboardKeyName, FoundPlayerPawn);
            UEnvQueryItemType_Actor::SetContextHelper(ContextData, FoundPlayerPawn);
        }
    }
}

APawn* UEnvQueryContext_ForceSetPlayerPawn::FindTargetPlayerPawn(const UWorld* World, const APawn* QuerierPawn, FDistanceComparePredicate IsBetterDistance) const
{
    if (IsValid(World) == false || IsValid(QuerierPawn) == false)
    {
        return nullptr;
    }
    
    APawn* TargetPlayerPawn = nullptr;
    float BestDistanceSq = 0.0f;
    bool bFoundFirstPlayer = false;

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = It->Get();
        if (IsValid(PlayerController) == false)
        {
            continue;
        }
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (IsValid(PlayerPawn) == false)
        {
            continue;
        }

        const float CurrentDistanceSq = FVector::DistSquared(QuerierPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
        if (bFoundFirstPlayer == false)
        {
            BestDistanceSq = CurrentDistanceSq;
            TargetPlayerPawn = PlayerPawn;
            bFoundFirstPlayer = true;
        }
        else if (IsBetterDistance(CurrentDistanceSq, BestDistanceSq))
        {
            BestDistanceSq = CurrentDistanceSq;
            TargetPlayerPawn = PlayerPawn;
        }
    }
    return TargetPlayerPawn;
}
