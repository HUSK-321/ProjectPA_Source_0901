#include "PlayGameState.h"
#include "EngineUtils.h"
#include "WeatherSystemComponent.h"
#include "WeatherSystemDynamicSky.h"

APlayGameState::APlayGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	WeatherSystemComponent = CreateDefaultSubobject<UWeatherSystemComponent>(TEXT("WeatherSystemComponent"));
	// 60분을 1일으로 잡고 진행한다
	Game10MinToRealSeconds = 60.0f / (144.0f) * 60.0f; // = 25.0f
}

void APlayGameState::BeginPlay()
{
	Super::BeginPlay();

	LoadGameTime();

	// TODO : Time Go to Component
	if (AWeatherSystemDynamicSky* SkySystem = GetWeatherSystemActor())
	{
		OnGameHourMinChanged.AddUObject(SkySystem, &AWeatherSystemDynamicSky::OnTimeChanged);
		if (WeatherSystemComponent)
		{
			WeatherSystemComponent->OnWeatherChanged.AddUObject(SkySystem, &AWeatherSystemDynamicSky::SetWeatherTag);
		}
	}
}

void APlayGameState::LoadGameTime()
{
	// TODO : 추후 저장되는 시간으로
	OnGameTimeChanged.Broadcast(GameTimeInfo);
	OnGameHourMinChanged.Broadcast(GameTimeInfo.Hour, GameTimeInfo.Min);
}

void APlayGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulateGameTime += DeltaTime;
	if(AccumulateGameTime > Game10MinToRealSeconds)
	{
		AccumulateGameTime -= Game10MinToRealSeconds;
		Add10Min();
	}
}

void APlayGameState::Add10Min()
{
	GameTimeInfo.Min += 10;
	if(GameTimeInfo.Min >= 60)
	{
		GameTimeInfo.Hour += 1;
		GameTimeInfo.Min = 0;

		if(GameTimeInfo.Hour >= 24)
		{
			GameTimeInfo.Day += 1;
			GameTimeInfo.Hour = 0;

			if(GameTimeInfo.IsMonthOver())
			{
				GameTimeInfo.Month += 1;
				GameTimeInfo.Day = 1;

				if(GameTimeInfo.Month > 12)
				{
					GameTimeInfo.Year += 1;
					GameTimeInfo.Month = 1;
				}
			}
		}
	}

	OnGameTimeChanged.Broadcast(GameTimeInfo);
	OnGameHourMinChanged.Broadcast(GameTimeInfo.Hour, GameTimeInfo.Min);
}

float APlayGameState::GetTemperature() const
{
	return (IsValid(WeatherSystemComponent)) ? WeatherSystemComponent->GetCurTemperature() : 0.0f;
}

AWeatherSystemDynamicSky* APlayGameState::GetWeatherSystemActor() const
{
	if (!GetWorld())
	{
		return nullptr;
	}

	for (TActorIterator<AWeatherSystemDynamicSky> It(GetWorld()); It; ++It)
	{
		return *It;
	}

	return nullptr;
}
