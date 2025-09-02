// All copyrights for this code are owned by Aster.

#include "WeatherSystemComponent.h"
#include "PlayGameState.h"
#include "Net/UnrealNetwork.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Generals/PAGameplayTags.h"

// Begin ESeasonWeatherInfo
FGameplayTag FSeasonWeatherInfo::GetWeather() const
{
	// 이상 기후 확률 판단
	const bool bAbnormalWeather = FMath::RandRange(0, 1000) < ExtremePercentage;
	if (bAbnormalWeather && WeatherPercentages.IsEmpty() == false)
	{
		const int32 RandomIndex = FMath::RandRange(0, ExtremeWeatherList.Num() - 1);
		return ExtremeWeatherList[RandomIndex];
	}

	// 일반 날씨 중 전체 비중 구하기
	int32 TotalWeatherPercentage = 0;
	for (auto& WeatherPercentage : WeatherPercentages)
	{
		TotalWeatherPercentage += WeatherPercentage.Percentage;
	}

	// 랜덤한 일반 확률 반환
	int32 RandomPercentage = FMath::RandRange(0, TotalWeatherPercentage - 1);
	for (int32 i = 0; i < WeatherPercentages.Num(); ++i)
	{
		RandomPercentage -= WeatherPercentages[i].Percentage;
		if (RandomPercentage <= 0)
		{
			return WeatherPercentages[i].WeatherTag;
		}
	}

	return Weather_Normal_Sunny;
}
// ~End ESeasonWeatherInfo

UWeatherSystemComponent::UWeatherSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UWeatherSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeatherSystemComponent, CurTemperature);
	DOREPLIFETIME(UWeatherSystemComponent, TodayWeather);
};

void UWeatherSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UWeatherSystemComponent::PostBeginPlay);
	}
}

void UWeatherSystemComponent::PostBeginPlay()
{
	// Bind Time System With Component
	APlayGameState* GameState = Cast<APlayGameState>(GetOwner());
	if (GameState && GameState->HasAuthority())
	{
		GameState->OnGameTimeChanged.AddUObject(this, &UWeatherSystemComponent::OnGameTimeChanged);
		OnGameTimeChanged(GameState->GetGameTimeInfo());
	}
}

void UWeatherSystemComponent::OnGameTimeChanged(const FGameTimeInfo& GameTimeInfo)
{
	// 1일이 지난 뒤 최고, 최저 기온을 계산해야 함
	if (GameTimeInfo.Min == 0 || TodayWeather.IsValid() == false)
	{
		if (GameTimeInfo.Hour == 0 || TodayWeather.IsValid() == false)
		{
			if (GameTimeInfo.Day == 1 || TodayWeather.IsValid() == false)
			{
				CalculateCurSeason(GameTimeInfo.Month);
			}
			CalculateTodayWeatherInfo(GameTimeInfo);
		}
		// 1시간 마다 현재 기온을 계산해야 합니다
		// 현재 시각에 따른 기온을 Curve 에서 가져오기
		if (WeatherDayTemperatureCurve.Contains(TodayWeather) == false || WeatherDayTemperatureCurve[TodayWeather] == nullptr)
		{
			PALog_E(TEXT("Wrong Weather in WeatherDayTemperatureCurve : %s"), *TodayWeather.ToString());
			return;
		}
		const float NormalizedTemperature = WeatherDayTemperatureCurve[TodayWeather]->GetFloatValue(GameTimeInfo.Hour);
		CurTemperature = NormalizedTemperature * TodayHighTemperature + (1.0f - NormalizedTemperature) * TodayLowTemperature;

		OnRep_CurTemperature();
	}
}

void UWeatherSystemComponent::CalculateTodayWeatherInfo(const FGameTimeInfo& GameTimeInfo)
{
	// 날씨 보기
	TodayWeather = SeasonWeatherInfo[CurSeason].GetWeather();
	OnRep_TodayWeather();

	const int8 MonthIndex = GameTimeInfo.Month - 1;
	// 현재 나올 수 있는 오차 생각
	const float TodayHighDeviation = FMath::RandRange(0.0f, MonthlyDayTemperatureRange[MonthIndex].HighDeviation);
	const float TodayLowDeviation = FMath::RandRange(0.0f, MonthlyDayTemperatureRange[MonthIndex].LowDeviation);
	
	// 만약 이상 기후라면 분석 후 온도 override 하고 처리하기
	const bool bAbnormalWeather = ExtremeDayTemperatureRange.Contains(TodayWeather);
	if (bAbnormalWeather)
	{
		TodayHighTemperature = ExtremeDayTemperatureRange[TodayWeather].HighTemperature + TodayHighDeviation;
		TodayLowTemperature = ExtremeDayTemperatureRange[TodayWeather].LowTemperature + TodayLowDeviation;
		return;
	}
	
	// 오늘 기온 결정
	TodayHighTemperature = MonthlyDayTemperatureRange[MonthIndex].HighTemperature + TodayHighDeviation;
	TodayLowTemperature = MonthlyDayTemperatureRange[MonthIndex].LowTemperature + TodayLowDeviation;
}

void UWeatherSystemComponent::CalculateCurSeason(const int8 CurMonth)
{
	switch (CurMonth)
	{
	case 12:
	case 1:
	case 2:
		CurSeason = ESeason::Winter;
		break;
	case 3:
	case 4:
		CurSeason = ESeason::Spring;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		CurSeason = ESeason::Summer;
		break;
	case 10:
	case 11:
		CurSeason = ESeason::Autumn;
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid month: %d"), CurMonth);
		CurSeason = ESeason::Winter;
		return;
	}
}

void UWeatherSystemComponent::OnRep_CurTemperature()
{
	OnTemperatureChanged.Broadcast(CurTemperature);
}

void UWeatherSystemComponent::OnRep_TodayWeather()
{
	OnWeatherChanged.Broadcast(TodayWeather);
}
