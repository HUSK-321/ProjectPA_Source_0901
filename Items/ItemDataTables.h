
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ProjectPA/Generals/PATypes.h"
#include "ItemDataTables.generated.h"

enum class EWeaponFireType : uint8;
class UStaticMesh;
enum class ESurvivalStats : uint8;

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

	/* 아이템의 사용 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;
	/* 아이템 아이콘 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;
	/* 아이템 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText ItemName;
	/* 아이템 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription;
	/* 아이템 무게 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Weight;
	/* 아이템 내구도의 남은 퍼센티지  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta=(ClampMax = 1))
	float ItemRemainPercentage;
	
	FItemDataTable()
		: ItemType(EItemType::Default)
		, Weight(0.1f)
		, ItemRemainPercentage(1.f)
	{}

	virtual TSoftObjectPtr<UStaticMesh> GetItemMesh() const { return nullptr; }
	virtual UAnimMontage* GetMontageToPlay() const { return nullptr; }
	virtual FTransform GetItemAttachTransform() const { return FTransform(); }
};

class UWeaponFireBase;

USTRUCT(BlueprintType)
struct FWeaponItemDataTable : public FItemDataTable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UAnimMontage> AttackMontage;
	/** 무기 메쉬 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UStaticMesh> WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	FTransform AttachTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	float AttackAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	float PartAttackAmount;
	/** 공격 1회당 소모할 내구도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	int32 AttackUsePercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponFireType FireType;

	FWeaponItemDataTable()
		: FItemDataTable()
		, AttackRange(50.f), AttackAngle(90.f), AttackDamage(10.f), PartAttackAmount(10.f), AttackUsePercentage(10)
	{
		ItemType = EItemType::Weapon;
	}

	virtual TSoftObjectPtr<UStaticMesh> GetItemMesh() const override { return WeaponMesh; }
	virtual UAnimMontage* GetMontageToPlay() const override { return !AttackMontage.IsNull() ? AttackMontage.LoadSynchronous() : nullptr; }
	virtual FTransform GetItemAttachTransform() const override { return AttachTransform; }
};

USTRUCT(BlueprintType)
struct FConsumeEffect
{
	GENERATED_BODY()

public:
	/** 영향을 줄 Attribute */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consume Effect")
	FGameplayAttribute Attribute;
	/** 적용할 값 (Additive) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consume Effect")
	float Value;

	FConsumeEffect() : Value(0.0f) {}
};

USTRUCT(BlueprintType)
struct FConsumeItemDataTable : public FItemDataTable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UAnimMontage> ConsumeMontage;
	/** 소비 아이템이 변경할 Attribute 값 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consume", meta = (AllowPrivateAccess = true))
	TArray<FConsumeEffect> ConsumeEffects;
	/** 아이템의 유통기한 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consume", meta = (AllowPrivateAccess = true))
	FGameTimeInfo ExpireDate;

	FConsumeItemDataTable()
		: FItemDataTable()
	{
		ItemType = EItemType::Consumable;
	}

	float GetConsumeEffect(const FGameplayAttribute& InAttribute) const
	{
		for (const FConsumeEffect& Modifier : ConsumeEffects)
		{
			if (Modifier.Attribute == InAttribute)
			{
				return Modifier.Value;
			}
		}
		return 0.0f;
	}

	virtual UAnimMontage* GetMontageToPlay() const override { return !ConsumeMontage.IsNull() ? ConsumeMontage.LoadSynchronous() : nullptr; }
};


USTRUCT(BlueprintType)
struct FBagItemDataTable : public FItemDataTable
{
	GENERATED_BODY()

public:
	/** 해당 가방 아이템이 최대로 가질 수 있는 무게 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag", meta = (AllowPrivateAccess = true))
	float MaxWeight;
	/** 가방이 포함할 수 없는 아이템 타입들 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag", meta = (AllowPrivateAccess = true))
	TArray<EItemType> CannotContainTypes;
	
	FBagItemDataTable()
		: FItemDataTable()
		, MaxWeight(0.0f)
	{
		ItemType = EItemType::Bag;
	}
};

USTRUCT(BlueprintType)
struct FKeyItemDataTable : public FItemDataTable
{
	GENERATED_BODY()

public:
	/** 현재 열쇠에 맞는 Code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key", meta = (AllowPrivateAccess = true))
	int32 KeyCode;

	FKeyItemDataTable()
		:
		FItemDataTable()
		, KeyCode(-1)
	{
		
	}
};

enum class EWearCategory : uint8;

USTRUCT(BlueprintType)
struct FWearingItemDataTable : public FItemDataTable
{
	GENERATED_BODY()

public:
	/**
	 * 옷의 보온성 정도, 두꺼운 옷일수록 +, 얇은 옷일수록 - 로 설정하면 됩니다.
	 * (입은 옷의 보온성 총합) + (현재 온도) 를 해 이게 0에 가까우면 문제가 없으며 차이가 날 수록 디버프에 걸리게 됩니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wearing")
	float ClothingWarmth;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Wearing", meta = (Bitmask, BitmaskEnum = EWearCategory))
	uint8 WearCategory;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Wearing", meta = (Bitmask, BitmaskEnum = EBodyPart))
	uint8 ProtectBodyPart;

	FWearingItemDataTable()
		: FItemDataTable()
		, ClothingWarmth(0.0f)
		, WearCategory(0)
		, ProtectBodyPart(0)
	{
		ItemType = EItemType::Wearing;
	}
};
