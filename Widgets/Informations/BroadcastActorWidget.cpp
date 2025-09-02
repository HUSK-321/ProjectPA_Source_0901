// All copyrights for this code are owned by Aster.


#include "BroadcastActorWidget.h"
#include "Components/TextBlock.h"

void UBroadcastActorWidget::SetBroadcastText(const FText& InBroadcasterName, const FText& InBroadcastText)
{
	if (BroadcastText)
	{
		BroadcastText->SetText(InBroadcastText);
	}
}
