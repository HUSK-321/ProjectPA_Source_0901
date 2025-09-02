// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "StatusEffectListElement.generated.h"

class UStatusGameplayEffect;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTPA_API UStatusEffectListElement : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> StatusIcon;

public:
	void SetElement(const UStatusGameplayEffect* InStatusEffect);
};
