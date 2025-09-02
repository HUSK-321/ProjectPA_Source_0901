// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "PlayerWidget.generated.h"

class UEnvironmentStatusWidget;
class UPlayerStatusWidget;
class UStatusEffectListWidget;
class UStatusGameplayEffect;

/**
 * 인게임 내 메인 플레이어 위젯으로 단순히 Blueprint 에서 레이아웃을 정렬하고 각 위젯에 대한 Return 함수를 가질 뿐입니다.
 * 실제 위젯별 동작은 각 위젯 클래스에서 진행하게 되며 해당 클래스에는 큰 기능 없이 현 상태를 유지해주세요.
 */
UCLASS()
class PROJECTPA_API UPlayerWidget : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEnvironmentStatusWidget> EnvironmentStatusWidget;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPlayerStatusWidget> PlayerStatusWidget;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatusEffectListWidget> StatusEffectListWidget;

public:
	UEnvironmentStatusWidget* GetEnvironmentStatusWidget() { return EnvironmentStatusWidget; }
	UPlayerStatusWidget* GetPlayerStatusWidget() { return PlayerStatusWidget; }
	UStatusEffectListWidget* GetStatusEffectListWidget() { return StatusEffectListWidget; }

	void OnStatusEffectListChanged(const TArray<const UStatusGameplayEffect*>& NewList);
};
