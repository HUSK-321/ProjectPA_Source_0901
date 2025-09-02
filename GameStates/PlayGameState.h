#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ProjectPA/Generals/PATypes.h"
#include "PlayGameState.generated.h"

class UWeatherSystemComponent;
class AWeatherSystemDynamicSky;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameTimeChanged, const FGameTimeInfo&)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameHourMinChanged, const int32 /** Hour */, const int32 /** Min */)

UCLASS()
class PROJECTPA_API APlayGameState : public AGameState
{
	GENERATED_BODY()

public:
	FOnGameTimeChanged OnGameTimeChanged;
	FOnGameHourMinChanged OnGameHourMinChanged;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UWeatherSystemComponent> WeatherSystemComponent;
	/** 게임의 10분이 실제 몇 초일지 지정 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Time, meta=(AllowPrivateAccess=true))
	float Game10MinToRealSeconds;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Time, meta=(AllowPrivateAccess=true))
	FGameTimeInfo GameTimeInfo;
	float AccumulateGameTime;
	
public:
	APlayGameState();
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE const FGameTimeInfo& GetGameTimeInfo() const { return GameTimeInfo; }
	float GetTemperature() const;
	
protected:
	virtual void BeginPlay() override;

private:	
	void LoadGameTime();
	void Add10Min();

	AWeatherSystemDynamicSky* GetWeatherSystemActor() const;
};
