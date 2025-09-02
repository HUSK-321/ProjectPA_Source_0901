// All copyrights for this code are owned by Aster.


#include "EnvQueryGenerator_PlayerCharacter.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

UEnvQueryGenerator_PlayerCharacter::UEnvQueryGenerator_PlayerCharacter()
{
	ItemType = UEnvQueryItemType_Actor::StaticClass(); 
}

void UEnvQueryGenerator_PlayerCharacter::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	UWorld* World = QueryInstance.World;
	if (IsValid(World) == false)
	{
		return;
	}

	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (IsValid(PlayerController))
		{
			APawn* PlayerPawn = PlayerController->GetPawn();
			if (IsValid(PlayerPawn))
			{
				QueryInstance.AddItemData<UEnvQueryItemType_Actor>(PlayerPawn);
			}
		}
	}
}

FText UEnvQueryGenerator_PlayerCharacter::GetDescriptionTitle() const
{
	return FText::FromString(TEXT("All Player Characters"));
}

FText UEnvQueryGenerator_PlayerCharacter::GetDescriptionDetails() const
{
	return FText::FromString(TEXT("Generates an EQS item for each valid player character (Pawn) in the world."));
}
