// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProjectPA/Items/ItemDataTables.h"
#include "ItemComparisonRules.generated.h"

/**
 * 아이템 비교 규칙의 베이스 클래스입니다.
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, Abstract)
class PROJECTPA_API UItemComparisonRuleBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 두 아이템을 비교하여 A가 B보다 더 나은지 판단합니다.
	 * @return A가 더 나으면 true를 반환합니다.
	 */
	virtual bool Compare(const FItemEncodedInfo& A, const FItemDataTable* DataA,
						 const FItemEncodedInfo& B, const FItemDataTable* DataB) const
	{
		return false;
	}

#if WITH_EDITORONLY_DATA
	/**
	 * Behavior Tree 에디터에 표시될 규칙에 대한 설명입니다.
	 */
	virtual FString GetComparisonDescription() const
	{
		return TEXT("None Rule");
	}
#endif
};

/**
 * 더 높은 공격력을 가진 무기를 찾는 규칙입니다.
 */
UCLASS()
class PROJECTPA_API UGetGreaterDamageWeaponRule : public UItemComparisonRuleBase
{
	GENERATED_BODY()

public:
	virtual bool Compare(const FItemEncodedInfo& A, const FItemDataTable* DataA,
						 const FItemEncodedInfo& B, const FItemDataTable* DataB) const override
	{
		if (DataB == nullptr)
		{
			return DataA != nullptr && DataA->ItemType == EItemType::Weapon;
		}
    
		if (DataA == nullptr || DataA->ItemType != EItemType::Weapon)
		{
			return false;
		}

		const FWeaponItemDataTable* WeaponDataA = static_cast<const FWeaponItemDataTable*>(DataA);
		const FWeaponItemDataTable* WeaponDataB = static_cast<const FWeaponItemDataTable*>(DataB);

		if (WeaponDataB && WeaponDataB->ItemType == EItemType::Weapon)
		{
			return WeaponDataA->AttackDamage > WeaponDataB->AttackDamage;
		}
	
		return true;
	}

#if WITH_EDITORONLY_DATA
	virtual FString GetComparisonDescription() const override
	{
		return TEXT("Highest Damage");
	}
#endif
};

UENUM(BlueprintType)
enum class EItemComparisonLogic : uint8
{
    None,
    HighestDamage     UMETA(DisplayName = "Highest Damage"),
};

namespace ItemComparator
{
    namespace Rules
    {
        static bool Compare_HighestDamage(const FItemEncodedInfo& A, const FItemDataTable* DataA, const FItemEncodedInfo& B, const FItemDataTable* DataB)
        {
            if (DataB == nullptr) return DataA != nullptr && DataA->ItemType == EItemType::Weapon;
            if (DataA == nullptr || DataA->ItemType != EItemType::Weapon) return false;

            const auto* WeaponDataA = static_cast<const FWeaponItemDataTable*>(DataA);
            const auto* WeaponDataB = static_cast<const FWeaponItemDataTable*>(DataB);

            if (WeaponDataB && WeaponDataB->ItemType == EItemType::Weapon)
            {
                return WeaponDataA->AttackDamage > WeaponDataB->AttackDamage;
            }
            return true;
        }
    }

    static bool Compare(EItemComparisonLogic Logic, const FItemEncodedInfo& A, const FItemDataTable* DataA, const FItemEncodedInfo& B, const FItemDataTable* DataB)
    {
        switch (Logic)
        {
            case EItemComparisonLogic::HighestDamage:
                return Rules::Compare_HighestDamage(A, DataA, B, DataB);
            case EItemComparisonLogic::None:
            default:
                return false;
        }
    }

#if WITH_EDITORONLY_DATA
    static FString GetDescription(EItemComparisonLogic Logic)
    {
        switch (Logic)
        {
            case EItemComparisonLogic::HighestDamage: return TEXT("Highest Damage");
            default: return TEXT("None");
        }
    }
#endif
}
