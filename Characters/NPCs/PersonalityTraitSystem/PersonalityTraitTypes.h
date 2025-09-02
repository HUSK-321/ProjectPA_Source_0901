// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PersonalityTraitTypes.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FPersonalityTraits
{
	GENERATED_BODY()
	/**
	 * 이타심의 정도를 나타냅니다.
	 * 0은 완전 이기적, 100은 완전 이타적입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta=(UIMin="0", UIMax="100"))
	int32 Altruism = 50;
	/**
	 * 낙관주의의 정도를 나타냅니다.
	 * 0은 완전 비관적, 100은 완전 낙관적입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta=(UIMin="0", UIMax="100"))
	int32 Optimism = 50;
	/**
	 * 용기의 정도를 나타냅니다.
	 * 0은 완전 신중함(겁쟁이), 100은 완전 용감(무모함)입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta=(UIMin="0", UIMax="100"))
	int32 Courage = 50;
	/**
	 * 타인에 대한 신뢰의 정도를 나타냅니다.
	 * 0은 완전 불신, 100은 완전 신뢰입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta=(UIMin="0", UIMax="100"))
	int32 Trust = 50;
	/**
	 * 호전성(공격성)의 정도를 나타냅니다.
	 * 0은 완전 평화주의, 100은 매우 호전적입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta=(UIMin="0", UIMax="100"))
	int32 Aggression = 50;
	/**
	 * 지능(논리적 사고)의 정도를 나타냅니다.
	 * 0은 완전 직관적, 100은 완전 지능적입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta=(UIMin="0", UIMax="100"))
	int32 Intellect = 50;
	/**
	 * 외향성의 정도를 나타냅니다.
	 * 0은 완전 내향적, 100은 완전 외향적입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality", meta=(UIMin="0", UIMax="100"))
	int32 Extroversion = 50;
};

/**
 * NPC가 플레이어에 대해 느끼는 감정이나 태도를 정의하는 구조체입니다. 모든 값들은 최대, 최소를 가지며 게임의 기획에 따라 조절합니다.
 */
USTRUCT(Blueprintable, BlueprintType)
struct FToPlayerRelationships
{
	GENERATED_BODY()
	
	#define RELATIONSHIP_MAX 100
	#define RELATIONSHIP_MID 50
	#define RELATIONSHIP_MIN 0
	
	/** 플레이어에 대한 호감도 및 애정입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta=(UIMin="0", UIMax="100"))
	int32 Affection = 0;
	/** 플레이어에 대한 신뢰도입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta=(UIMin="0", UIMax="100"))
	int32 Trust = 0;
	/** 플레이어에 대한 두려움입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta=(UIMin="0", UIMax="100"))
	int32 Fear = 0;
	/** 플레이어에 대한 존경심입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta=(UIMin="0", UIMax="100"))
	int32 Respect = 0;
	/** 플레이어에 대한 원한 또는 적대감입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship", meta=(UIMin="0", UIMax="100"))
	int32 Grudge = 0;
};

UENUM(BlueprintType)
enum class EPersonalityActionType : uint8
{
	DialoguePositive,		// 플레이어에게 좋은 이야기를 할지 여부
	ActionHelpPlayer,		// 플레이어를 도울지 여부
	ActionDemandResource,	// 플레이어의 아이템을 탐낼 경우
	ActionAttack,			// 플레이어를 공격할 경우
	ActionAvoid,			// 플레이어를 기피할 경우 
    
	MAX						UMETA(Hidden)
};

/** 위 EPersonalityActionType 을 배열로 하는 구조체입니다. */
USTRUCT(BlueprintType)
struct FActionWeightsArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Weights")
	TArray<int32> Weights;

	FActionWeightsArray()
	{
		Weights.Init(0, static_cast<int32>(EPersonalityActionType::MAX));
	}

	FORCEINLINE int32& operator[](const EPersonalityActionType ActionType)
	{
		return Weights[static_cast<int32>(ActionType)];
	}
    
	FORCEINLINE const int32& operator[](const EPersonalityActionType ActionType) const
	{
		return Weights[static_cast<int32>(ActionType)];
	}
};

/**
 * @brief 플레이어가 NPC에게 취할 수 있는 행동들을 정의하는 열거형입니다.
 * 이 값은 UReactionCalculator의 인풋으로 사용됩니다.
 */
UENUM(BlueprintType)
enum class EPlayerAction : uint8
{
	/** 기본값: 아무 행동도 하지 않음 */
	None		UMETA(DisplayName = "None"),

	/** 긍정적 상호작용 */
	GiveItem	UMETA(DisplayName = "Give Item"),
	Praise		UMETA(DisplayName = "Praise"),
	Help		UMETA(DisplayName = "Help"),
    
	/** 부정적 상호작용 */
	Attack		UMETA(DisplayName = "Attack"),
	Insult		UMETA(DisplayName = "Insult"),
	Steal		UMETA(DisplayName = "Steal"),

	/** 중립적 상호작용 */
	Talk		UMETA(DisplayName = "Talk"),
	Trade		UMETA(DisplayName = "Trade"),
};
