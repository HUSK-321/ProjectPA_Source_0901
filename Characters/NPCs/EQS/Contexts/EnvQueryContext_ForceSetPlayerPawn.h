// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_ForceSetPlayerPawn.generated.h"

UENUM(BlueprintType)
enum class EPlayerSearchType : uint8
{
	/** 현재 AI에게 가장 가까운 플레이어를 찾습니다. */
	Closest UMETA(DisplayName = "Closest Player"),
	/** 현재 AI에게 가장 먼 플레이어를 찾습니다. */
	Farthest UMETA(DisplayName = "Farthest Player")
};

/**
 * 
 */
UCLASS()
class PROJECTPA_API UEnvQueryContext_ForceSetPlayerPawn : public UEnvQueryContext
{
	GENERATED_BODY()

private:
	/**
	 * 가장 가까운 플레이어 폰을 저장할 블랙보드 키의 이름입니다.
	 * 이 키는 AActor* 타입의 값을 저장할 수 있어야 합니다.
	 */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BlackboardKeyName;
	/**
 * 어떤 플레이어를 찾을지 결정합니다 (가장 가까운 또는 가장 먼).
 */
	UPROPERTY(EditAnywhere, Category = Search)
	EPlayerSearchType SearchType;

public:
	UEnvQueryContext_ForceSetPlayerPawn();

	//~Begin UEnvQueryContext interfaces
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	//~End UEnvQueryContext interfaces

private:
	using FDistanceComparePredicate = TFunction<bool(const float CurrentDistance, const float BestDistance)>;
	APawn* FindTargetPlayerPawn(const UWorld* World, const APawn* QuerierPawn, 
								FDistanceComparePredicate IsBetterDistance) const;
};
