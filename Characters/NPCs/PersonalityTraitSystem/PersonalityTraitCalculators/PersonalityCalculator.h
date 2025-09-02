// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Characters/NPCs/PersonalityTraitSystem/PersonalityTraitTypes.h"
#include "UObject/Object.h"
#include "PersonalityCalculator.generated.h"

class IPersonalityTraitInteractable;

UENUM(BlueprintType)
enum class EComparisonOperator : uint8
{
	EqualTo					UMETA(DisplayName = "=="),
	NotEqualTo				UMETA(DisplayName = "!="),
	GreaterThan				UMETA(DisplayName = ">"),
	LessThan				UMETA(DisplayName = "<"),
	GreaterThanOrEqualTo	UMETA(DisplayName = ">="),
	LessThanOrEqualTo		UMETA(DisplayName = "<="),
};

UENUM(BlueprintType)
enum class EAttributeContainerType : uint8
{
	Personality		UMETA(DisplayName = "Personality Traits"),
	Relationships	UMETA(DisplayName = "Player Relationships")
};

USTRUCT(Blueprintable, BlueprintType)
struct FPersonalityOperator
{
	GENERATED_BODY()

public:
	/** 아래 변수가 어떤 구조체에 속하는지를 나타냅니다. */
	UPROPERTY(EditAnywhere)
	EAttributeContainerType AttributeContainerType;
	/**
	 * 비교할 대상 변수의 이름입니다. (예: "AltruismVsSelfInterest", "Fear")
	 * 이 이름은 FPersonalityTraits 또는 FPlayerRelationships 구조체의 멤버 변수 이름과 정확히 일치해야 합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comparison")
	FName VariableName;
	/**
	 * 비교할 연산자 (VariableName (ComparisionOperator) CompareValue)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comparison")
	EComparisonOperator ComparisonOperator;
	/**
	 * 이 값을 고정값으로 사용할지, 아니면 다른 변수의 값을 가져와 비교할지 결정합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comparison")
	bool bUseFixedValue = true;
	/**
	 * bUseFixedValue가 true일 때 사용될 고정 값입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comparison", meta=(EditCondition="bUseFixedValue", EditConditionHides))
	int32 FixedValue = 0;
	/**
	 * bUseFixedValue가 false일 때 비교할 다른 변수의 이름입니다.
	 * 이 변수는 현재 개체의 다른 성격 또는 관계 변수가 될 수 있습니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comparison", meta=(EditCondition="!bUseFixedValue", EditConditionHides))
	FName OtherVariableName;
};

USTRUCT(Blueprintable, BlueprintType)
struct FActionWeightElement
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	EPersonalityActionType ActionType;
	UPROPERTY(EditAnywhere)
	int32 Weight;
};

/** FBehaviourCalculateOperator 에서 연산이 성공했을 때 Target 에 추가할 값 */
USTRUCT(Blueprintable, BlueprintType)
struct FResultValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FActionWeightElement> ActionWeightElements;
	UPROPERTY(EditAnywhere)
	FToPlayerRelationships ToPlayerRelationships;
};

USTRUCT(Blueprintable, BlueprintType)
struct FCalculation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FPersonalityOperator Operator;
	UPROPERTY(EditAnywhere)
	FResultValue Result;
};

enum class EPlayerAction : uint8;
/**
 * 플레이어의 행동에 따라 NPC 의 가중치 증감도를 계산하는 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTPA_API UPersonalityCalculator : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TMap<EPlayerAction, FCalculation> CalculateRule;

private:
	bool EvaluateComparison(TScriptInterface<IPersonalityTraitInteractable> TargetObject, const FPersonalityOperator& Operator) const;

	static TOptional<int32> GetValueFromTarget(const TScriptInterface<IPersonalityTraitInteractable>& TargetObject,
	                                           const FName& VariableName, const EAttributeContainerType ContainerType);
	static FName GetContainerName(const EAttributeContainerType ContainerType);
	static UScriptStruct* GetScriptStruct(const EAttributeContainerType ContainerType);
};
