// All copyrights for this code are owned by Aster.


#include "PlayerAttributeSet.h"
#include "Net/UnrealNetwork.h"

UPlayerAttributeSet::UPlayerAttributeSet(const FObjectInitializer& ObjectInitializer)
	:
	Super(ObjectInitializer)
{
	InitStamina(100.0f);
	InitMaxStamina(100.0f);
	InitHunger(0.0f);
	InitThirsty(0.0f);
	InitStress(0.0f);
	InitTired(0.0f);
	InitSleep(0.0f);
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Stamina, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Hunger, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Thirsty, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Stress, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Tired, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Sleep, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, TemperatureDeviation, COND_OwnerOnly, REPNOTIFY_Always);
}

void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	AttributeChangedEvent.Broadcast(Attribute);
}

#define ADD_REPLICATION_FUNCTION(Attribute, RepFunction) \
ReplicationFunctionMap.Add({ Attribute, FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::RepFunction) });

void UPlayerAttributeSet::BindHealthComponentEnd()
{
	ADD_REPLICATION_FUNCTION(GetHealthAttribute(), OnRep_Health);
	ADD_REPLICATION_FUNCTION(GetMaxHealthAttribute(), OnRep_MaxHealth);
	ADD_REPLICATION_FUNCTION(GetMoveSpeedAttribute(), OnRep_MoveSpeed);
	ADD_REPLICATION_FUNCTION(GetSprintSpeedAttribute(), OnRep_SprintSpeed);
	ADD_REPLICATION_FUNCTION(GetSpeedRatioAttribute(), OnRep_SpeedRatio);
	ADD_REPLICATION_FUNCTION(GetStaminaAttribute(), OnRep_Stamina);
	ADD_REPLICATION_FUNCTION(GetMaxStaminaAttribute(), OnRep_MaxStamina);
	ADD_REPLICATION_FUNCTION(GetHungerAttribute(), OnRep_Hunger);
	ADD_REPLICATION_FUNCTION(GetThirstyAttribute(), OnRep_Thirsty);
	ADD_REPLICATION_FUNCTION(GetStressAttribute(), OnRep_Stress);
	ADD_REPLICATION_FUNCTION(GetTiredAttribute(), OnRep_Tired);
	ADD_REPLICATION_FUNCTION(GetSleepAttribute(), OnRep_Sleep);

	ADD_REPLICATION_FUNCTION(GetHeadPartAttribute(), OnRep_HeadPart);
	ADD_REPLICATION_FUNCTION(GetBodyPartAttribute(), OnRep_BodyPart);
	ADD_REPLICATION_FUNCTION(GetLeftArmPartAttribute(), OnRep_LeftArmPart);
	ADD_REPLICATION_FUNCTION(GetRightArmPartAttribute(), OnRep_RightArmPart);
	ADD_REPLICATION_FUNCTION(GetLeftLegPartAttribute(), OnRep_LeftLegPart);
	ADD_REPLICATION_FUNCTION(GetRightLegPartAttribute(), OnRep_RightLegPart);
	
	OnRep_Health(Health);
	OnRep_Stamina(Stamina);

	OnRep_HeadPart(GetHeadPart());
	OnRep_BodyPart(GetBodyPart());
	OnRep_RightArmPart(GetRightArmPart());
	OnRep_LeftArmPart(GetLeftArmPart());
	OnRep_RightLegPart(GetRightArmPart());
	OnRep_LeftLegPart(GetLeftLegPart());
}

#undef ADD_REPLICATION_FUNCTION

void UPlayerAttributeSet::OnPlayerMoved(const int32 MovedMeter)
{
	if (GetOwningAbilitySystemComponent() == nullptr || GetOwningAbilitySystemComponent()->IsOwnerActorAuthoritative() == false)
	{
		return;
	}

	const float CurStamina = Stamina.GetCurrentValue();
	// TODO : Magic Number to class field
	SetStamina(CurStamina - (MovedMeter * 0.1f));
}

#define REPLICATE_IF_NOT_AUTHORITY(Attribute, OldValue) \
if (GetOwningActor() && !GetOwningActor()->HasAuthority()) \
{ \
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Attribute, OldValue); \
}

void UPlayerAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	REPLICATE_IF_NOT_AUTHORITY(Stamina, OldStamina);
    if (GetStamina() <= 0.0f)
    {
        SetSprinting(false);
    }
    StaminaAttributeChangedEvent.Broadcast(OldStamina.GetCurrentValue(), GetStamina());
    OnStaminaChangedEvent.Broadcast(GetMaxStamina(), GetStamina());
}

void UPlayerAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	REPLICATE_IF_NOT_AUTHORITY(MaxStamina, OldMaxStamina);
    MaxStaminaAttributeChangedEvent.Broadcast(OldMaxStamina.GetCurrentValue(), GetMaxStamina());
    OnStaminaChangedEvent.Broadcast(GetMaxStamina(), GetStamina());
}

void UPlayerAttributeSet::OnRep_Hunger(const FGameplayAttributeData& OldHunger) const
{
	REPLICATE_IF_NOT_AUTHORITY(Hunger, OldHunger);
    HungerAttributeChangedEvent.Broadcast(OldHunger.GetCurrentValue(), GetHunger());
}

void UPlayerAttributeSet::OnRep_Thirsty(const FGameplayAttributeData& OldThirsty) const
{
	REPLICATE_IF_NOT_AUTHORITY(Thirsty, OldThirsty);
    ThirstyAttributeChangedEvent.Broadcast(OldThirsty.GetCurrentValue(), GetThirsty());
}

void UPlayerAttributeSet::OnRep_Stress(const FGameplayAttributeData& OldStress) const
{
	REPLICATE_IF_NOT_AUTHORITY(Stress, OldStress);
    StressAttributeChangedEvent.Broadcast(OldStress.GetCurrentValue(), GetStress());
}

void UPlayerAttributeSet::OnRep_Tired(const FGameplayAttributeData& OldTired) const
{
	REPLICATE_IF_NOT_AUTHORITY(Tired, OldTired);
    TiredAttributeChangedEvent.Broadcast(OldTired.GetCurrentValue(), GetTired());
}

void UPlayerAttributeSet::OnRep_Sleep(const FGameplayAttributeData& OldSleep) const
{
	REPLICATE_IF_NOT_AUTHORITY(Sleep, OldSleep);
    SleepAttributeChangedEvent.Broadcast(OldSleep.GetCurrentValue(), GetSleep());
}

void UPlayerAttributeSet::OnRep_TemperatureDeviation(const FGameplayAttributeData& OldTemperatureDeviation) const
{
	REPLICATE_IF_NOT_AUTHORITY(TemperatureDeviation, OldTemperatureDeviation);
	TemperatureDeviationAttributeChangedEvent.Broadcast(OldTemperatureDeviation.GetCurrentValue(), GetTemperatureDeviation());
}
#undef REPLICATE_IF_NOT_AUTHORITY
