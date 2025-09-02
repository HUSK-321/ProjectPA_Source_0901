// All copyrights for this code are owned by Aster.


#include "BTTask_GameplayEvent.h"
#include "AIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectPA/Characters/GameCharacterBase.h"

EBTNodeResult::Type UBTTask_GameplayEvent::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AGameCharacterBase* OwnerCharacter = AIController->GetPawn<AGameCharacterBase>();
	if (!OwnerCharacter)
	{
		return EBTNodeResult::Failed;
	}
	
	if (UAbilitySystemComponent* OwnerASC = OwnerCharacter->GetAbilitySystemComponent())
	{
		FGameplayEventData Payload;
		Payload.EventTag = GameplayEventTag;
		Payload.Instigator = OwnerCharacter;
		OwnerASC->HandleGameplayEvent(GameplayEventTag, &Payload);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
