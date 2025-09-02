#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PATypes.generated.h"

USTRUCT(BlueprintType)
struct FGameTimeInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int32 Year;
	UPROPERTY(EditAnywhere)
	int8 Month;
	UPROPERTY(EditAnywhere)
	int8 Day;
	UPROPERTY(EditAnywhere)
	int8 Hour;
	UPROPERTY(EditAnywhere)
	int8 Min;
	
	bool IsMonthOver() const { return DayInMonths[Month - 1] < Day; }
	FString ToString() const 
	{
		return FString::Printf(TEXT("%04d.%02d.%02d %02d:%02d"), Year, Month, Day, Hour, Min);
	}

	FGameTimeInfo operator+(const FGameTimeInfo& Other) const
	{
		FGameTimeInfo Result = *this;

		Result.Min += Other.Min;
		if (Result.Min >= 60)
		{
			Result.Hour += Result.Min / 60;
			Result.Min %= 60;
		}

		Result.Hour += Other.Hour;
		if (Result.Hour >= 24)
		{
			Result.Day += Result.Hour / 24;
			Result.Hour %= 24;
		}

		Result.Day += Other.Day;
		while (Result.Day > DayInMonths[Result.Month - 1])
		{
			Result.Day -= DayInMonths[Result.Month - 1];
			Result.Month++;

			if (Result.Month > 12)
			{
				Result.Month = 1;
				Result.Year++;
			}
		}

		Result.Month += Other.Month;
		if (Result.Month > 12)
		{
			Result.Year += (Result.Month - 1) / 12;
			Result.Month = (Result.Month - 1) % 12 + 1;
		}
		Result.Year += Other.Year;

		return Result;
	}
	
private:
	static constexpr int DayInMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
};

static FGameTimeInfo INVALID_GAME_TIME = { .Year = INDEX_NONE, .Month = INDEX_NONE, .Day = INDEX_NONE, .Hour = INDEX_NONE, .Min = INDEX_NONE };
static FGameTimeInfo GameStartTime = FGameTimeInfo{ .Year = 2025, .Month = 1, .Day = 1, .Hour = 8, .Min = 0 };

UENUM()
enum class ESeason
{
	Spring	UMETA(DisplayName = "Spring"),
	Summer	UMETA(DisplayName = "Summer"),
	Autumn	UMETA(DisplayName = "Autumn"),
	Winter	UMETA(DisplayName = "Winter")
};

//================== Item Types

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Default		UMETA(DisplayName = "Default Normal"),
	Weapon		UMETA(DisplayName = "Weapon"),
	Consumable	UMETA(DisplayName = "Consumable"),
	Wearing		UMETA(DisplayName = "Wearing"),
	Bag			UMETA(DisplayName = "Bag"),
	Quest		UMETA(DisplayName = "Quest"),

	MAX			UMETA(DisplayName = "DefaultMAX")
};

UENUM()
enum class EGetItemError : uint8
{
	OutOfWeight			UMETA(DisplayName = "OutOfWeight"),
	NotCompatibility	UMETA(DisplayName = "NotCompatibility"),
	GeneralError		UMETA(DisplayName = "GeneralError"),
	NotValidBagIndex	UMETA(DisplayName = "NotValidBagIndex"),
	
	NoError					UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class EWearCategory : uint8
{
	None		= 0			UMETA(DisplayName = "None"),
	Clothes		= 1			UMETA(DisplayName = "Clothes"),
	Arms		= 1 << 1	UMETA(DisplayName = "Arms"),
	Gloves		= 1 << 2	UMETA(DisplayName = "Gloves"),
	Outerwear	= 1 << 3	UMETA(DisplayName = "Outerwear"),
	Head		= 1 << 4	UMETA(DisplayName = "Head"),
	Face		= 1 << 5	UMETA(DisplayName = "Face"),
	Shoes		= 1 << 6	UMETA(DisplayName = "Shoes"),
	Pants		= 1 << 7	UMETA(DisplayName = "Pants"),
};
ENUM_CLASS_FLAGS(EWearCategory);
static constexpr int32 WearCategoryCount = 8;

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor="true"))
enum class EBodyPart : uint8
{
	None		= 0			UMETA(Hidden),
	Head		= 1			UMETA(DisplayName = "Head"),
	Body		= 1 << 1	UMETA(DisplayName = "Body"),
	LeftArm		= 1 << 2	UMETA(DisplayName = "LeftArms"),
	RightArm	= 1 << 3	UMETA(DisplayName = "RightArms"),
	LeftLeg		= 1 << 4	UMETA(DisplayName = "LeftLeg"),
	RightLeg	= 1 << 5	UMETA(DisplayName = "RightLeg"),

	MAX			= 1 << 7	UMETA(DisplayName = "MAX"),
};
ENUM_CLASS_FLAGS(EBodyPart);

static EBodyPart GetRandomBodyPart(bool bIsUpper)
{
	const int32 NumBits = bIsUpper ? 4 : 2;
	const int32 Index = FMath::RandRange(0, NumBits - 1);

	const uint8 StartBit = bIsUpper ? 0 : 4;
	const uint8 BitValue = 1 << (StartBit + Index);

	return static_cast<EBodyPart>(BitValue);
}

/** Health, Stamina 를 제외한 수치들은 높을수록 좋지 않은 효과 */
UENUM(Blueprintable)
enum class ESurvivalStats : uint8
{
	Health		UMETA(DisplayName = "Health"),
	Stamina		UMETA(DisplayName = "Stamina"),
	Hunger		UMETA(DisplayName = "Hunger"),
	Thirsty		UMETA(DisplayName = "Thirsty"),
	Stress		UMETA(DisplayName = "Stress"),
	Tired		UMETA(DisplayName = "Tired"),
	Sleep		UMETA(DisplayName = "Sleep"),
	
	MAX UMETA(DisplayName = "DefaultMax")
};

/**
 * 아이템에 대한 정보를 가지는 구조체 입니다. 아이템의 테이블 상 키 값, 내구도, 제조일자 등의 스냅샷 정보들을 가지게 됩니다.
 */
USTRUCT(BlueprintType)
struct FItemEncodedInfo
{
	GENERATED_BODY()

private:
	/** const variables */
	static constexpr uint32 ItemIDMultiplier      = 1000;
	static constexpr uint32 PercentageMultiplier  = 10000000;
	static constexpr uint32 YearMultiplier        = 10000;
	static constexpr uint32 MonthMultiplier       = 100;

	/**
	 * 아이템의 Key 정보가 들어 있습니다. 00_000 형식이며 {EItemType}_{Item Num In DataTable} 입니다
	 */
	UPROPERTY(EditAnywhere)
	//FName ItemKey;
	uint32 EncodedItemKey;
	/**
	 * 아이템의 내구도, 제조일자 정보가 들어있습니다. 100(내구도) / 000 년 00 월 00 일 (년도는 2000 년이 더해집니다)
	 */
	UPROPERTY(EditAnywhere)
	uint32 EncodedPercentageDate;

public:

	FItemEncodedInfo() : EncodedItemKey(0), EncodedPercentageDate(0) {}

	explicit FItemEncodedInfo(EItemType ItemType, int32 ItemID, const FGameTimeInfo& InMadeDate, float InRemainPercentage = 1.0f)
	{
		EncodedItemKey = (static_cast<uint32>(ItemType) * ItemIDMultiplier) + (ItemID % ItemIDMultiplier);

		const uint32 ItemRemainPercentage = static_cast<uint32>(100 * InRemainPercentage);
		EncodedPercentageDate = ItemRemainPercentage * PercentageMultiplier 
						 + (InMadeDate.Year % 1000) * YearMultiplier 
						 + (InMadeDate.Month * MonthMultiplier) 
						 + (InMadeDate.Day);
	}

	explicit FItemEncodedInfo(const FName& InItemKey, const FGameTimeInfo& InMadeDate, float InRemainPercentage = 1.0f)
	{
		EncodedItemKey = 0;
		const uint32 ItemRemainPercentage = static_cast<uint32>(100 * InRemainPercentage);
		EncodedPercentageDate = ItemRemainPercentage * PercentageMultiplier 
						 + (InMadeDate.Year % 1000) * YearMultiplier 
						 + (InMadeDate.Month * MonthMultiplier) 
						 + (InMadeDate.Day);
		
		if (InItemKey.IsNone())
		{
			return;
		}

		FString KeyString = InItemKey.ToString();
		FString TypePrefix, ItemIDString;

		if (KeyString.Split(TEXT("_"), &TypePrefix, &ItemIDString))
		{
			const TOptional<EItemType> ItemTypeOpt = GetTypeFromString(TypePrefix);
			if (ItemTypeOpt.IsSet())
			{
				const int32 ItemID = FCString::Atoi(*ItemIDString);
				EncodedItemKey = (static_cast<uint32>(ItemTypeOpt.GetValue()) * ItemIDMultiplier) + (ItemID % ItemIDMultiplier);
			}
		}
	}

	void Serialize(FArchive& Ar)
	{
		Ar << EncodedItemKey;
		Ar << EncodedPercentageDate;
	}
	
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar.SerializeIntPacked(EncodedItemKey);
		Ar.SerializeIntPacked(EncodedPercentageDate);
        
		bOutSuccess = true;
		return true;
	}

	bool operator==(const FItemEncodedInfo& Other) const
	{
		return	EncodedItemKey == Other.EncodedItemKey && 
				EncodedPercentageDate == Other.EncodedPercentageDate;
	}
	
	// Getter Functions
	
	FName GetItemKey() const
	{
		if (EncodedItemKey == 0)
		{
			return NAME_None;
		}

		const EItemType ItemType = static_cast<EItemType>(EncodedItemKey / 1000);
		const int32 ItemID = EncodedItemKey % 1000;
        
		const TOptional<FString> TypePrefixOpt = GetTypePrefixString(ItemType);

		if (TypePrefixOpt.IsSet())
		{
			const FString FormattedKey = FString::Printf(TEXT("%s_%03d"), *TypePrefixOpt.GetValue(), ItemID);
			return FName(*FormattedKey);
		}
        
		return NAME_None;
	}
	
	int32 GetItemRemainPercentage() const
	{
		return EncodedPercentageDate / 10000000;
	}

	void SetItemRemainPercentage(const int32 InRemainPercentage)
	{
		const int32 DateInfo = EncodedPercentageDate % 10000000;
		EncodedPercentageDate = InRemainPercentage * 10000000 + DateInfo;
	}
	
	FGameTimeInfo GetItemMadeDate() const
	{
		return FGameTimeInfo{ .Year = (EncodedPercentageDate / 10000) % 1000 + 2000, 
							  .Month = static_cast<int8>((EncodedPercentageDate / 100) % 100), 
							  .Day = static_cast<int8>(EncodedPercentageDate % 100)	};
	}

	bool IsValid() const
	{
		return EncodedItemKey > 0 && GetItemRemainPercentage() > 0;
	}

private:
	static TOptional<FString> GetTypePrefixString(EItemType ItemType)
	{
		switch (ItemType)
		{
		case EItemType::Weapon:		return FString(TEXT("Weapon"));
		case EItemType::Consumable: return FString(TEXT("Consume"));
		case EItemType::Wearing:	return FString(TEXT("Wearing"));
		default:
			return {};
		}
	}

	static TOptional<EItemType> GetTypeFromString(const FString& TypePrefix)
	{
		if (TypePrefix.Equals(TEXT("Weapon"), ESearchCase::IgnoreCase)) return EItemType::Weapon;
		if (TypePrefix.Equals(TEXT("Consume"), ESearchCase::IgnoreCase)) return EItemType::Consumable;
		if (TypePrefix.Equals(TEXT("Wearing"), ESearchCase::IgnoreCase)) return EItemType::Wearing;
        
		return {};
	}
};

template<>
struct TStructOpsTypeTraits<FItemEncodedInfo> : public TStructOpsTypeTraitsBase2<FItemEncodedInfo>
{
	enum 
	{
		WithNetSerializer = true,
		WithIdenticalViaEquality = true
	};
};
