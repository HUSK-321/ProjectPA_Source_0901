// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "WeatherSystemComponent.generated.h"

enum class ESeason;
class UCurveFloat;
struct FGameTimeInfo;

/** 월별 날씨/기온 정보들 */
USTRUCT()
struct FTemperatureInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bOverrideTemperature;
	/** 최고 기온 */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bOverrideTemperature"))
	float HighTemperature;
	/** 최저 기온 */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bOverrideTemperature"))
	float LowTemperature;
	/** 최고 기온 편차 */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bOverrideTemperature"))
	float HighDeviation;
	/** 최저 기온 편차 */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bOverrideTemperature"))
	float LowDeviation;
	/** 위 설정을 사용하지 않을 때  */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bOverrideTemperature"))
	FGameplayTag TemperatureWeatherTag;

	FTemperatureInfo()
		: bOverrideTemperature(true),
		HighTemperature(0.0f),
		LowTemperature(0.0f),
		HighDeviation(0.0f),
		LowDeviation(0.0f)
	{
	}
};

USTRUCT()
struct FWeatherPercentage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 Percentage;
	UPROPERTY(EditAnywhere)
	FGameplayTag WeatherTag;
};

USTRUCT()
struct FSeasonWeatherInfo
{
	GENERATED_BODY()

public:
	/** 이상 기후가 될 수 있는 확률 1000 이면 100%, 511 이면 51.1% 라는 뜻 */
	UPROPERTY(EditAnywhere)
	int32 ExtremePercentage;
	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> ExtremeWeatherList;
	UPROPERTY(EditAnywhere)
	TArray<FWeatherPercentage> WeatherPercentages;

	FGameplayTag GetWeather() const;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTemperatureChanged, const float /** CurTemperature */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, const FGameplayTag& /** WeatherTag */)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTPA_API UWeatherSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnTemperatureChanged OnTemperatureChanged;
	FOnWeatherChanged OnWeatherChanged;

private:
	UPROPERTY(Transient, VisibleAnywhere, ReplicatedUsing = OnRep_CurTemperature, Category = "Temperature")
	float CurTemperature;
	UPROPERTY(Transient, VisibleAnywhere, Category = "Temperature")
	float TodayHighTemperature;
	UPROPERTY(Transient, VisibleAnywhere, Category = "Temperature")
	float TodayLowTemperature;
	UPROPERTY(Transient, VisibleAnywhere, Category = "Temperature")
	ESeason CurSeason;
	/** 현재 날씨 캐싱 */
	UPROPERTY(Transient, VisibleAnywhere, ReplicatedUsing = OnRep_TodayWeather, Category = "Temperature")
	FGameplayTag TodayWeather;
	
	/** 날씨 별 일교차를 나타내는 Curve 그래프 (날씨, 일교차 그래프)
	 *	일교차 그래프는 0 ~ 1 로 나타내지며 1이면 최고 기온, 0이면 최저 기온을 의미합니다.
	 */
	UPROPERTY(EditAnywhere, Category = "Temperature")
	TMap<FGameplayTag, UCurveFloat*> WeatherDayTemperatureCurve;
	/**
	 * 월별 최고/최저 기온을 나타내는 배열
	 */
	UPROPERTY(EditAnywhere, Category = "Temperature")
	TArray<FTemperatureInfo> MonthlyDayTemperatureRange;
	/**
	 * 이상 기온일 시 최고/최저 기온을 나타내는 배열
	 */
	UPROPERTY(EditAnywhere, Category = "Temperature")
	TMap<FGameplayTag, FTemperatureInfo> ExtremeDayTemperatureRange;
	/**
	 * 계절에 따른 날씨 확률 정보
	 */
	UPROPERTY(EditAnywhere, Category = "Temperature")
	TMap<ESeason, FSeasonWeatherInfo> SeasonWeatherInfo;

public:
	UWeatherSystemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE float GetCurTemperature() const { return CurTemperature; }

protected:
	virtual void BeginPlay() override;
	void PostBeginPlay();

private:
	void OnGameTimeChanged(const FGameTimeInfo& GameTimeInfo);

	void CalculateTodayWeatherInfo(const FGameTimeInfo& GameTimeInfo);
	void CalculateCurSeason(const int8 CurMonth);

	UFUNCTION()
	void OnRep_CurTemperature();
	UFUNCTION()
	void OnRep_TodayWeather();
};
