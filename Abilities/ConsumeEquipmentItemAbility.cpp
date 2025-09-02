// All copyrights for this code are owned by Aster.


#include "ConsumeEquipmentItemAbility.h"

#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Components/EquipmentComponent.h"

const FItemEncodedInfo* UConsumeEquipmentItemAbility::GetTargetItemInfo() const
{
	if (AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(GetActorInfo().AvatarActor))
	{
		if (UEquipmentComponent* EquipmentComponent = OwnerCharacter->FindComponentByClass<UEquipmentComponent>())
		{
			return &EquipmentComponent->GetEquippingItem();
		}
	}
	return nullptr;
}

FItemEncodedInfo* UConsumeEquipmentItemAbility::GetTargetItemInfo()
{
	if (AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(GetActorInfo().AvatarActor))
	{
		if (UEquipmentComponent* EquipmentComponent = OwnerCharacter->FindComponentByClass<UEquipmentComponent>())
		{
			return &EquipmentComponent->GetEquippingItem_Ref();
		}
	}
	return nullptr;
}

FItemDataTable* UConsumeEquipmentItemAbility::GetTargetItemData() const
{
	if (AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(GetActorInfo().AvatarActor))
	{
		if (UEquipmentComponent* EquipmentComponent = OwnerCharacter->FindComponentByClass<UEquipmentComponent>())
		{
			return EquipmentComponent->GetEquippingItemData();
		}
	}
	return nullptr;
}

void UConsumeEquipmentItemAbility::DestroyTargetItem()
{
	if (AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(GetActorInfo().AvatarActor))
	{
		if (UEquipmentComponent* EquipmentComponent = OwnerCharacter->FindComponentByClass<UEquipmentComponent>())
		{
			EquipmentComponent->DestroyEquippedItem();
		}
	}
}
