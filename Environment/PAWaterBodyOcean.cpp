// All copyrights for this code are owned by Aster.

#include "PAWaterBodyOcean.h"
#include "ProjectPA/Characters/GameCharacterBase.h"

APAWaterBodyOcean::APAWaterBodyOcean()
{
	
}

void APAWaterBodyOcean::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (AGameCharacterBase* GameCharacter = Cast<AGameCharacterBase>(OtherActor))
	{
		GameCharacter->StartSwimming(this);
	}
}

void APAWaterBodyOcean::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (AGameCharacterBase* GameCharacter = Cast<AGameCharacterBase>(OtherActor))
	{
		GameCharacter->StopSwimming(this);
	}
}
