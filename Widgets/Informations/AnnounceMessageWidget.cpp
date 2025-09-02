// All copyrights for this code are owned by Aster.


#include "AnnounceMessageWidget.h"
#include "Components/TextBlock.h"

void UAnnounceMessageWidget::AddAnnounce(const FString& InMessage)
{
	AddAnnounce(FText::FromString(InMessage));
}

void UAnnounceMessageWidget::AddAnnounce(const FText& InMessage)
{
	AnnounceText->SetText(InMessage);

	if (SetVisible)
	{
		if (GetWorld())
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(AnnounceHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(AnnounceHandle);
			}
			GetWorld()->GetTimerManager().SetTimer(AnnounceHandle, this, &UAnnounceMessageWidget::ClearAnnounce, 2.f);
		}
		PlayAnimation(SetVisible);
	}
}

void UAnnounceMessageWidget::ClearAnnounce()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AnnounceHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(AnnounceHandle);
	}
	
	if (SetVisible)
	{
		PlayAnimation(SetVisible, 0, 1, EUMGSequencePlayMode::Reverse);
	}
}
