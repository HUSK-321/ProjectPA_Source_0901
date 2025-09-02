// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "UEnvQueryContext_GetBlackboardActor.generated.h"

/**
 * AI의 BlackboardComponent 에서 Key 를 통해 가져와 Actor 로 반환합니다.
 */
UCLASS(Blueprintable)
class PROJECTPA_API UUEnvQueryContext_GetBlackboardActor : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	UUEnvQueryContext_GetBlackboardActor(const FObjectInitializer& ObjectInitializer);

	//~Begin UEnvQueryContext interfaces
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	//~End UEnvQueryContext interfaces
	
protected:
	/**
	 * 블랙보드에서 Actor를 가져올 키의 이름입니다.
	 * 이 키는 AActor* 타입의 값을 저장하고 있어야 합니다.
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BlackboardKeyName;
};
