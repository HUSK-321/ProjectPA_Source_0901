// All copyrights for this code are owned by Aster.


#include "BTDecorator_CheckEquippingItemType.h"
#include "AIController.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Components/EquipmentComponent.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"
#include "ProjectPA/Items/ItemDataTables.h"

UBTDecorator_CheckEquippingItemType::UBTDecorator_CheckEquippingItemType()
{
	NodeName = TEXT("Check Equipping Item Type");
	ItemTypeToCheck = EItemType::Default;
}

bool UBTDecorator_CheckEquippingItemType::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return false;
	}

	const AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(AIController->GetPawn());
	if (OwnerCharacter == nullptr)
	{
		return false;
	}

	const UEquipmentComponent* EquipmentComponent = OwnerCharacter->GetEquipmentComponent();
	if (EquipmentComponent == nullptr)
	{
		return false;
	}

	const FItemEncodedInfo& EquippingItemInfo = EquipmentComponent->GetEquippingItem();
	if (EquippingItemInfo.IsValid() == false)
	{
		return false;
	}

	const UWorld* World = OwnerCharacter->GetWorld();
	if (World == nullptr)
	{
		return false;
	}

	const IGameDataTableProvider* DataProvider = Cast<IGameDataTableProvider>(World->GetGameInstance());
	if (DataProvider == nullptr)
	{
		return false;
	}

	const FItemDataTable* ItemData = DataProvider->GetItemDataByKey(EquippingItemInfo.GetItemKey());
	if (ItemData == nullptr)
	{
		return false;
	}

	return ItemData->ItemType == ItemTypeToCheck;
}

#if WITH_EDITORONLY_DATA
FString UBTDecorator_CheckEquippingItemType::GetStaticDescription() const
{
	return FString::Printf(TEXT("Equipping Item Is: %s"), *UEnum::GetValueAsString(ItemTypeToCheck));
}
#endif