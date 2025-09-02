// All copyrights for this code are owned by Aster.


#include "PABaseViewModel.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPABaseViewModel::Initialize(UObject* OwningObject)
{
}

APlayerController* UPABaseViewModel::GetOwningPlayer() const
{
	if (const UUserWidget* OwningWidget = Cast<UUserWidget>(GetOuter()))
	{
		return OwningWidget->GetOwningPlayer();
	}

	return UGameplayStatics::GetPlayerController(this, 0);
}
