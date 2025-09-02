// All copyrights for this code are owned by Aster.


#include "EnvQueryContext_FindCharacter.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "Kismet/GameplayStatics.h"

#include "ProjectPA/Characters/GameCharacterBase.h"

void UEnvQueryContext_FindCharacter::ProvideContext(FEnvQueryInstance& Instance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(Instance, ContextData);

	if (Instance.Owner.IsValid() == false)
	{
		return;
	}
	AActor* OwnerActor = Cast<AActor>(Instance.Owner.Get());
	if (IsValid(OwnerActor) == false)
	{
		return;
	}

	const FVector AILocation = OwnerActor->GetActorLocation();
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(OwnerActor->GetWorld(), SearchCharacterClass, FoundCharacters);

	AActor* NearestActor = nullptr;
	float MinDistanceSq = FLT_MAX;
	
	for (AActor* CharacterActor : FoundCharacters)
	{
		if (IsValid(CharacterActor) && CharacterActor != OwnerActor)
		{
			const float CurrentDistanceSq = FVector::DistSquared(AILocation, CharacterActor->GetActorLocation());

			if (CurrentDistanceSq < MinDistanceSq)
			{
				MinDistanceSq = CurrentDistanceSq;
				NearestActor = CharacterActor;
			}
		}
	}

	if (NearestActor)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, NearestActor);
	}

}
