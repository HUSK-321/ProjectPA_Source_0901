// All copyrights for this code are owned by Aster.


#include "EnvQueryTest_AngleFromTargetFront.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "ProjectPA/PALog.h"

UEnvQueryTest_AngleFromTargetFront::UEnvQueryTest_AngleFromTargetFront()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();

	ScoringEquation = EEnvTestScoreEquation::Linear;
}

void UEnvQueryTest_AngleFromTargetFront::RunTest(FEnvQueryInstance& QueryInstance) const
{
    TArray<AActor*> TargetActors;
    QueryInstance.PrepareContext(TargetContext, TargetActors);

    if (TargetActors.IsEmpty())
    {
        return;
    }
    

    const FVector TargetLocation = TargetActors[0]->GetActorLocation();
    FVector TargetForwardDirection = TargetActors[0]->GetActorForwardVector();
    TargetForwardDirection.Z = 0.f;
    TargetForwardDirection.Normalize();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
        FVector DirectionToItem = ItemLocation - TargetLocation;
        DirectionToItem.Z = 0.f;
        DirectionToItem.Normalize();

        const float DotProduct = FVector::DotProduct(TargetForwardDirection, DirectionToItem);
        const float AngleRadians = FMath::Acos(DotProduct);
        const float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

        float Score = 0.0f;
        if (AngleScoringCurve)
        {
            Score = AngleScoringCurve->GetFloatValue(AngleDegrees);
        }
        else
        {
            Score = AngleDegrees / 180.0f;
        }
        It.SetScore(TestPurpose, FilterType, Score, 0.0f, 1.0f);
    }
}

FText UEnvQueryTest_AngleFromTargetFront::GetDescriptionTitle() const
{
    return FText::FromString(TEXT("Angle From Player"));
}

FText UEnvQueryTest_AngleFromTargetFront::GetDescriptionDetails() const
{
    FString Desc = FString::Printf(TEXT("Player의 정면에서의 각도 계산."));

    if (ScoringEquation == EEnvTestScoreEquation::Linear)
    {
        Desc += TEXT("(정면일 수록 값이 높아짐)");
    }
    else if (ScoringEquation == EEnvTestScoreEquation::InverseLinear)
    {
        Desc += TEXT("(정면일 수록 값이 낮아짐)");
    }

    if (AngleScoringCurve)
    {
        Desc += TEXT("스코어링 커브 사용 중: ");
        Desc += FString::Printf(TEXT("(커브 이름: %s)"), *AngleScoringCurve->GetName());
    }
    else
    {
        Desc += TEXT("스코어링 커브 없음: 0도에서 0.0, 180도에서 1.0으로 선형 스코어링.");
    }

    return FText::FromString(Desc);
}
