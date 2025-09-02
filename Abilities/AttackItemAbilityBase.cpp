// All copyrights for this code are owned by Aster.


#include "AttackItemAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Characters/CharacterAttributes/CharacterAttributeSet.h"
#include "ProjectPA/Items/ItemDataTables.h"
#include "ProjectPA/Items/WeaponFires/WeaponFireBase.h"

UAttackItemAbilityBase::UAttackItemAbilityBase()
{
}

void UAttackItemAbilityBase::ActualUseItem()
{
	AGameCharacterBase* AbilityOwner = Cast<AGameCharacterBase>(GetActorInfo().AvatarActor);
	if (AbilityOwner == nullptr)
	{
		return;
	}
	
	const FWeaponItemDataTable* WeaponItem = static_cast<const FWeaponItemDataTable*>(GetTargetItemData());
	if (WeaponItem == nullptr || WeaponItem->FireType == EWeaponFireType::None)
	{
		return;
	}
	
	TArray<FHitResult> HitResults;
	WeaponFire::Execute(WeaponItem->FireType, AbilityOwner, WeaponItem->AttackRange, WeaponItem->AttackAngle, HitResults);
	
	if (HitResults.IsEmpty())
	{
		return;
	}
	
	ApplyGameplayEventToTargets(HitResults, WeaponItem);
}

void UAttackItemAbilityBase::FinishAbility()
{
	if (K2_HasAuthority() == false)
	{
		return;
	}

	// 아이템의 내구도를 사용하기 
	FItemEncodedInfo* EquippingSlot = GetTargetItemInfo();
	if (EquippingSlot != nullptr && EquippingSlot->IsValid())
	{
		const FWeaponItemDataTable* WeaponItem = static_cast<const FWeaponItemDataTable*>(GetTargetItemData());
		if (WeaponItem)
		{
			const int32 AttackUseAmount = WeaponItem->AttackUsePercentage;
			const int32 NewRemainPercentage = EquippingSlot->GetItemRemainPercentage() - AttackUseAmount;

			if (NewRemainPercentage > 0)
			{
				EquippingSlot->SetItemRemainPercentage(NewRemainPercentage);
			}
			else
			{
				DestroyTargetItem();
			}
		}
	}
	
	Super::FinishAbility();
}


void UAttackItemAbilityBase::ApplyGameplayEventToTargets(const TArray<FHitResult>& HitResults, const FWeaponItemDataTable* WeaponItem)
{
	if (WeaponItem == nullptr)
	{
		return;
	}
	
	TSet<AActor*> DamagedActorSet;
	for (const FHitResult& Hit : HitResults)
	{
		AActor* TargetActor = Hit.GetActor();
		if (TargetActor == nullptr || DamagedActorSet.Contains(TargetActor))
		{
			continue;
		}
		
		const IAbilitySystemInterface* TargetAbilityActor = Cast<IAbilitySystemInterface>(TargetActor);
		if (TargetAbilityActor == nullptr)
		{
			continue;
		}
		UAbilitySystemComponent* TargetASC = TargetAbilityActor->GetAbilitySystemComponent();
		if (TargetASC == nullptr)
		{
			continue;
		}
		UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
		if (OwnerASC == nullptr)
		{
			continue;
		}
		
		if (UGameplayEffect* MadeEffect = NewObject<UGameplayEffect>(TargetASC))
		{
			FGameplayModifierInfo DamageModifier;
			DamageModifier.Attribute = UCharacterAttributeSet::GetHealthAttribute();
			DamageModifier.ModifierOp = EGameplayModOp::Additive;
			DamageModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(-WeaponItem->AttackDamage);
			MadeEffect->Modifiers.Add(DamageModifier);

			FGameplayModifierInfo DamagePartModifier;
			bool bIsUpperAttack = true;
			if (const AGameCharacterBase* AbilityOwner = Cast<AGameCharacterBase>(GetActorInfo().AvatarActor))
			{
				bIsUpperAttack = (AbilityOwner->bIsCrouched == false);
			}
			
			FGameplayModifierInfo PartAttackModifier;
			PartAttackModifier.Attribute = GetRandomDamagePart(bIsUpperAttack);
			PartAttackModifier.ModifierOp = EGameplayModOp::Additive;
			PartAttackModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(-WeaponItem->PartAttackAmount);
			MadeEffect->Modifiers.Add(PartAttackModifier);
			
			OwnerASC->ApplyGameplayEffectToTarget(MadeEffect, TargetASC);
			DamagedActorSet.Emplace(TargetActor);
		}		
	}
}

FGameplayAttribute UAttackItemAbilityBase::GetRandomDamagePart(bool bIsUpper)
{
	FGameplayAttribute HitPartAttribute;

	switch (const EBodyPart HitPart = GetRandomBodyPart(bIsUpper))
	{
	case EBodyPart::Head:       HitPartAttribute = UCharacterAttributeSet::GetHeadPartAttribute(); break;
	case EBodyPart::Body:       HitPartAttribute = UCharacterAttributeSet::GetBodyPartAttribute(); break;
	case EBodyPart::LeftArm:    HitPartAttribute = UCharacterAttributeSet::GetLeftArmPartAttribute(); break;
	case EBodyPart::RightArm:   HitPartAttribute = UCharacterAttributeSet::GetRightArmPartAttribute(); break;
	case EBodyPart::LeftLeg:    HitPartAttribute = UCharacterAttributeSet::GetLeftLegPartAttribute(); break;
	case EBodyPart::RightLeg:   HitPartAttribute = UCharacterAttributeSet::GetRightLegPartAttribute(); break;
	default:                    HitPartAttribute = UCharacterAttributeSet::GetBodyPartAttribute(); break;
	}
	return HitPartAttribute;
}
