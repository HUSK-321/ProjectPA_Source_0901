// All copyrights for this code are owned by Aster.


#include "PlayerWidget.h"
#include "StatusEffectListWidget.h"

void UPlayerWidget::OnStatusEffectListChanged(const TArray<const UStatusGameplayEffect*>& NewList)
{
	if (IsValid(StatusEffectListWidget))
	{
		StatusEffectListWidget->OnStatusEffectListChanged(NewList);
	}
}
