// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ItemComparisonRules.h"
#include "ProjectPA/Generals/PATypes.h"
#include "UObject/Object.h"
#include "AbilityItemPayloads.generated.h"

/**
 * 아이템 사용 시 Ability 에 OptionalObject 로 전달될 클래스
 */
UCLASS()
class PROJECTPA_API UAbilityItemPayloads : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Payload")
	FItemEncodedInfo ItemInfo;
	UPROPERTY(BlueprintReadWrite, Category="Payload")
	int32 OwnerUID = INDEX_NONE;
};

/**
 * BT Task에서 장착할 아이템에 대한 요청 정보를 담아 어빌리티에 전달하는 Payload입니다.
 */
UCLASS(BlueprintType)
class PROJECTPA_API UEquipItemQueryPayload : public UObject
{
	GENERATED_BODY()

public:
	/** 장착을 시도할 아이템의 타입입니다. */
	UPROPERTY(BlueprintReadWrite, Category = "Item Query")
	EItemType ItemType = EItemType::Weapon;

	/** '최고의' 아이템을 결정하는 규칙입니다. */
	UPROPERTY(BlueprintReadWrite, Category = "Item Query")
	EItemComparisonLogic ComparisonLogic;
};
