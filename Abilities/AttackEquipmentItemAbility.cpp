// All copyrights for this code are owned by Aster.


#include "AttackEquipmentItemAbility.h"

#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Components/EquipmentComponent.h"

const FItemEncodedInfo* UAttackEquipmentItemAbility::GetTargetItemInfo() const
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

FItemEncodedInfo* UAttackEquipmentItemAbility::GetTargetItemInfo()
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

FItemDataTable* UAttackEquipmentItemAbility::GetTargetItemData() const
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

void UAttackEquipmentItemAbility::DestroyTargetItem()
{
	if (AGameCharacterBase* OwnerCharacter = Cast<AGameCharacterBase>(GetActorInfo().AvatarActor))
	{
		if (UEquipmentComponent* EquipmentComponent = OwnerCharacter->FindComponentByClass<UEquipmentComponent>())
		{
			EquipmentComponent->DestroyEquippedItem();
		}
	}
}
