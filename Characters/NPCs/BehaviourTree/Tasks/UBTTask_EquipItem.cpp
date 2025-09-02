// All copyrights for this code are owned by Aster.


#include "UBTTask_EquipItem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"

#include "ProjectPA/Abilities/AbilityObjects/AbilityItemPayloads.h"
#include "ProjectPA/Characters/GameCharacterBase.h"

UUBTTask_EquipItem::UUBTTask_EquipItem()
	: ItemTypeToEquip(EItemType::Default)
	, ComparisonLogic(EItemComparisonLogic::None)
{
	NodeName = TEXT("Equip Item");
}

EBTNodeResult::Type UUBTTask_EquipItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !EventTag.IsValid())
	{
		return EBTNodeResult::Failed;
	}

	AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(AIController->GetPawn());
	if (!OwnerCharacter)
	{
		return EBTNodeResult::Failed;
	}

	UEquipItemQueryPayload* Payload = NewObject<UEquipItemQueryPayload>(OwnerCharacter);
	Payload->ItemType = ItemTypeToEquip;
	Payload->ComparisonLogic = ComparisonLogic;

	FGameplayEventData EventData;
	EventData.EventTag = EventTag;
	EventData.OptionalObject = Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerCharacter, EventTag, EventData);

	return EBTNodeResult::Succeeded;
}

#if WITH_EDITORONLY_DATA
FString UUBTTask_EquipItem::GetStaticDescription() const
{
	return FString::Printf(TEXT("Trigger Ability: %s\nItemType: %s\nRule: %s"),
		*EventTag.ToString(),
		*UEnum::GetValueAsString(ItemTypeToEquip),
		*ItemComparator::GetDescription(ComparisonLogic)); // static 함수 호출
}
#endif
