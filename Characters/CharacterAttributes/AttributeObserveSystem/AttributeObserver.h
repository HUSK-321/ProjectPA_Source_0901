// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttributeSet.h"
#include "ActiveGameplayEffectHandle.h"
#include "AttributeObserver.generated.h"

class UPlayerAttributeSet;
class UAbilityObserveEvent;
class UGameplayEffect;

UENUM(BlueprintType, Blueprintable)
enum class ObserveCalculator : uint8
{
	LessThan	UMETA(DisplayName = "<="),	
	MoreThan	UMETA(DisplayName = ">="),	
	Under		UMETA(DisplayName = "<"),	
	Over		UMETA(DisplayName = ">"),
	Equal		UMETA(DisplayName = "=="),
	NotEqual	UMETA(DisplayName = "!=")
};

USTRUCT(BlueprintType)
struct FObserveInfo
{
	GENERATED_BODY()

	/** 기준점 */
	UPROPERTY(EditAnywhere)
	float Threshold;
	/** 연산량, 위 기준점을 기준으로 계산하게 됩니다. (예 <= 일 경우 기준점 이하일때 측정) */
	UPROPERTY(EditAnywhere)
	ObserveCalculator Calculator;
	/** 기준점에 부응할 때 부여할 Effects */
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApply;

	TArray<FActiveGameplayEffectHandle> AppliedEffectHandles;
};

USTRUCT(BlueprintType)
struct FObserveInfos
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FObserveInfo> InfoList;
};

USTRUCT()
struct FAttributeStatus
{
	GENERATED_BODY()

	float Value;
	bool bEventApplied;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTPA_API UAttributeObserver : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayAttribute, FObserveInfos> AttributesToObserve;
	TMap<FGameplayAttribute, FAttributeStatus> PrevAttributeStatuses;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC;
	
public:
	void InitializeObserver(const UPlayerAttributeSet* PlayerAttributeSet);

private:
	void OnAttributeChanged(const FGameplayAttribute& ChangedAttribute);
	void CalculateAllEffects();
	bool CheckAttributeThreshold(const FObserveInfo& AttributeToObserve, const float CurrentValue);

	void ApplyEffects(FObserveInfo& AttributeToObserve);
	void RemoveEffects(FObserveInfo& AttributeToObserve);
};
