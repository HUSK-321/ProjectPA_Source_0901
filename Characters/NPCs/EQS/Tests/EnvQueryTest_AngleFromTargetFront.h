// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_AngleFromTargetFront.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UEnvQueryTest_AngleFromTargetFront : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_AngleFromTargetFront();
	
	//~Begin UEnvQueryTest
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
	//~End UEnvQueryTest

private:
	UPROPERTY(EditDefaultsOnly, Category = Test)
	TSubclassOf<UEnvQueryContext> TargetContext;
	/**
	 * 각도에 따른 스코어링 커브입니다. (옵션)
	 * 0도(정면)에서 180도(후면)까지의 각도에 대해 어떤 스코어를 부여할지 정의할 수 있습니다.
	 * 예를 들어, 0도에서 10점, 90도에서 50점, 180도에서 100점 등으로 설정 가능합니다.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Test)
	UCurveFloat* AngleScoringCurve;
};
