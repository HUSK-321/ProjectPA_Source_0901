#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "ProjectPA/Characters/CharacterAttributes/CharacterAttributeSet.h"
#include "ProjectPA/Generals/PATypes.h"
#include "PlayerHealthComponent.generated.h"

class UGameplayEffect;

UCLASS()
class PROJECTPA_API UPlayerHealthComponent : public UHealthComponent
{
	GENERATED_BODY()

public:
	FStatusChangedEvent OnStaminaChangedEvent;
	FBodyPartChanged OnBodyPartChangedEvent;
	FMovedDistanceChanged OnPlayerMovedEvent;

private:
	/** 가장 최근에 잔 시간 */
	FGameTimeInfo LastSleepTime;
	/** 플레이어가 움직인 거리. cm 단위로 1M 이상의 M 단위는 Tick 에서 계산되어 사라지게 관리 */
	float PrevMoveDistance;
	/** 위 PrevMoveDistance 를 계산하기 위해 기록하는 이전 Tick 에서의 위치 */
	FVector PrevLocation;
	/** 게임 시간 10분 지날 때 마다 부여할 GameplayEffect 들 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects, meta=(AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayEffect>> GameTimePeriodicEffects;

public:
	UPlayerHealthComponent();
	void SetupHealthWidget();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnHealthChanged(const float MaxHealth, const float CurHealth) override;
	void OnStaminaChanged(const float MaxStamina, const float CurStamina);
	
	//~Begin Change to Attribute
	virtual void OnHealthAttributeChanged(const float OldValue, const float NewValue) override;
	virtual void OnMaxHealthAttributeChanged(const float OldValue, const float NewValue) override;
	void SetPartValue(const EBodyPart InBodyPart, const float InCurrentValue, const float InMaxValue = 100.0f);
	//~End Change to Attribute
	
	virtual void StartDying() override;
	virtual void StartRest() override;
	virtual void StartSleep() override;

	// TODO : For Test
	void ToggleStatusPartWidget();

protected:
	virtual void BeginPlay() override;

private:
	void BindWidgetEvents();

	void OnGameTimeChanged(const FGameTimeInfo& NewTime);
};
