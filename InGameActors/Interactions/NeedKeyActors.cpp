// All copyrights for this code are owned by Aster.


#include "NeedKeyActors.h"
#include "ProjectPA/Characters/PlayerCharacter.h"
#include "ProjectPA/Items/KeyItemBase.h"
#include "ProjectPA/Characters/PLayerCharacter.h"
#include "ProjectPA/HUD/PlayGameHud.h"

ANeedKeyActors::ANeedKeyActors()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANeedKeyActors::InteractWith(APlayerCharacter* InteractingPlayer)
{
	if (InteractingPlayer == nullptr)
	{
		return;
	}

	UItemBase* FoundItem = nullptr;
	// if (UBagManagerComponent* BagComponent = InteractingPlayer->GetBagComponent())
	// {
	// 	FoundItem = BagComponent->HaveItem(KeyItemToNeed);
	// }

	if (FoundItem)
	{
		if (UKeyItemBase* KeyItem = Cast<UKeyItemBase>(FoundItem))
		{
			if (KeyItem->IsKeyCorrect(KeyCode))
			{
				Open();
				return;
			}
		}
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APlayGameHud* HUD = Cast<APlayGameHud>(PC->GetHUD()))
		{
			HUD->SetAnnounceMessage(TEXT("시스템 메세지 : 문에 맞는 열쇠가 없습니다"));
		}
	}
}

void ANeedKeyActors::Open_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Open"));
}
