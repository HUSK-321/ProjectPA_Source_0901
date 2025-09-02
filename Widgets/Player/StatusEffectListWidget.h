// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "StatusEffectListWidget.generated.h"

class UStatusEffectListElement;
class UWrapBox;
class UStatusGameplayEffect;

/**
 * 플레이어의 상태이상 목록을 나타내는 위젯
 */
UCLASS()
class PROJECTPA_API UStatusEffectListWidget : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWrapBox> EffectListBox;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UStatusEffectListElement> ListElementClass;
	
public:
	void OnStatusEffectListChanged(const TArray<const UStatusGameplayEffect*>& NewList);

private:
	UStatusEffectListElement* MakeElement(const UStatusGameplayEffect* BaseEffect);
};
