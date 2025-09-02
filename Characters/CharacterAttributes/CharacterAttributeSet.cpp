// All copyrights for this code are owned by Aster.


#include "CharacterAttributeSet.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectPA/Generals/PATypes.h"

UCharacterAttributeSet::UCharacterAttributeSet(const FObjectInitializer& ObjectInitializer)
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitMoveSpeed(300.0f);
	InitSprintSpeed(500.0f);
	InitCrouchSpeed(200.f);
	InitSpeedRatio(1.0f);

	InitHeadPart(100.0f);
	InitBodyPart(100.0f);
	InitLeftArmPart(100.0f);
	InitRightArmPart(100.0f);
	InitLeftLegPart(100.0f);
	InitRightLegPart(100.0f);
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (ReplicationFunctionMap.Find(Attribute) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Not handled attribute getted on %s  (Attribute : %s)"), *GetName(), *Attribute.AttributeName);
		return;
	}
	
	ReplicationFunctionMap[Attribute].ExecuteIfBound(FGameplayAttributeData());

	if (GetOwningActor())
	{
		GetOwningActor()->ForceNetUpdate();
	}
}

void UCharacterAttributeSet::SetSprinting(const bool bSprinting)
{
	bCharacterSprinting = bSprinting;
	OnRep_Sprinting();
}

void UCharacterAttributeSet::SetCrouching(const bool bCrouching)
{
	bCharacterCrouching = bCrouching;
	OnRep_Crouching();
}

void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
    }
    HealthAttributeChangedEvent.Broadcast(OldHealth.GetCurrentValue(), GetHealth());
    OnHealthChangedEvent.Broadcast(GetMaxHealth(), GetHealth());
}

void UCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHealth, OldMaxHealth);
    }
    MaxHealthAttributeChangedEvent.Broadcast(MaxHealth.GetCurrentValue(), GetMaxHealth());
    OnHealthChangedEvent.Broadcast(GetMaxHealth(), GetHealth());
}

void UCharacterAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MoveSpeed, OldMoveSpeed);
    }
    SetOwnerCharacterMoveSpeed();
    MoveSpeedAttributeChangedEvent.Broadcast(OldMoveSpeed.GetCurrentValue(), GetMoveSpeed());
}

void UCharacterAttributeSet::OnRep_SprintSpeed(const FGameplayAttributeData& OldSprintSpeed)
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, SprintSpeed, OldSprintSpeed);
    }
    SetOwnerCharacterMoveSpeed();
}

void UCharacterAttributeSet::OnRep_CrouchSpeed(const FGameplayAttributeData& OldCrouchSpeed)
{
	if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, CrouchSpeed, OldCrouchSpeed);
	}
	SetOwnerCharacterCrouch();
	SetOwnerCharacterMoveSpeed();
}

void UCharacterAttributeSet::OnRep_SpeedRatio(const FGameplayAttributeData& OldSpeedRatio)
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, SpeedRatio, OldSpeedRatio);
    }
    SetOwnerCharacterMoveSpeed();
}

void UCharacterAttributeSet::OnRep_HeadPart(const FGameplayAttributeData& OldHeadPart)
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, HeadPart, OldHeadPart);
    }
    OnBodyPartChanged.Broadcast(EBodyPart::Head, GetHeadPart(), 100.0f);
}

void UCharacterAttributeSet::OnRep_BodyPart(const FGameplayAttributeData& OldBodyPart)
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, BodyPart, OldBodyPart);
    }
    OnBodyPartChanged.Broadcast(EBodyPart::Body, GetBodyPart(), 100.0f);
}

void UCharacterAttributeSet::OnRep_LeftArmPart(const FGameplayAttributeData& OldLeftArmPart)
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, LeftArmPart, OldLeftArmPart);
    }
    OnBodyPartChanged.Broadcast(EBodyPart::LeftArm, GetLeftArmPart(), 100.0f);
}

void UCharacterAttributeSet::OnRep_RightArmPart(const FGameplayAttributeData& OldRightArmPart)
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, RightArmPart, OldRightArmPart);
    }
    OnBodyPartChanged.Broadcast(EBodyPart::RightArm, GetRightArmPart(), 100.0f);
}

void UCharacterAttributeSet::OnRep_LeftLegPart(const FGameplayAttributeData& OldLeftLegPart)
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, LeftLegPart, OldLeftLegPart);
    }
    OnBodyPartChanged.Broadcast(EBodyPart::LeftLeg, GetLeftLegPart(), 100.0f);
}

void UCharacterAttributeSet::OnRep_RightLegPart(const FGameplayAttributeData& OldRightLegPart)
{
    if (GetOwningActor() && GetOwningActor()->HasAuthority() == false)
    {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, RightLegPart, OldRightLegPart);
    }
    OnBodyPartChanged.Broadcast(EBodyPart::RightLeg, GetRightLegPart(), 100.0f);
}

void UCharacterAttributeSet::OnRep_Sprinting()
{
    SetOwnerCharacterMoveSpeed();
}

void UCharacterAttributeSet::OnRep_Crouching()
{
	SetOwnerCharacterCrouch();
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if (IsValid(OwnerCharacter) == false)
	{
		return;
	}

	bCharacterCrouching ? OwnerCharacter->Crouch() : OwnerCharacter->UnCrouch();
	SetOwnerCharacterMoveSpeed();
}

void UCharacterAttributeSet::SetOwnerCharacterMoveSpeed()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if (IsValid(OwnerCharacter) && OwnerCharacter->GetMovementComponent())
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = GetMaxMoveSpeedByStatus();
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = GetMaxMoveSpeedByStatus();
		if (OwnerCharacter->HasAuthority())
		{
			OwnerCharacter->ForceNetUpdate();
		}
	}
}

void UCharacterAttributeSet::SetOwnerCharacterCrouch()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if (IsValid(OwnerCharacter))
	{
		bCharacterCrouching ? OwnerCharacter->Crouch(true) : OwnerCharacter->UnCrouch(true);
		if (OwnerCharacter->HasAuthority())
		{
			OwnerCharacter->ForceNetUpdate();
		}
	}
}

float UCharacterAttributeSet::GetMaxMoveSpeedByStatus()
{
	const float OriginSpeed = bCharacterCrouching ? GetCrouchSpeed() :
													bCharacterSprinting ? GetSprintSpeed() : GetMoveSpeed();
	return OriginSpeed * GetSpeedRatio();
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, SprintSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, CrouchSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, SpeedRatio, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, bCharacterSprinting, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, bCharacterCrouching, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, HeadPart, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, BodyPart, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, LeftArmPart, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, RightArmPart, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, LeftLegPart, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, RightLegPart, COND_None, REPNOTIFY_Always);
}

void UCharacterAttributeSet::BindHealthComponentEnd()
{
	ReplicationFunctionMap.Add( { GetHealthAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_Health) } );
	ReplicationFunctionMap.Add( { GetMaxHealthAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_MaxHealth) } );
	ReplicationFunctionMap.Add( { GetMoveSpeedAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_MoveSpeed) } );
	ReplicationFunctionMap.Add( { GetSprintSpeedAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_SprintSpeed) } );
	ReplicationFunctionMap.Add( { GetCrouchSpeedAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_CrouchSpeed) } );
	ReplicationFunctionMap.Add( { GetSpeedRatioAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_SpeedRatio) } );

	ReplicationFunctionMap.Add( { GetHeadPartAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_HeadPart) } );
	ReplicationFunctionMap.Add( { GetBodyPartAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_BodyPart) } );
	ReplicationFunctionMap.Add( { GetLeftArmPartAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_LeftArmPart) } );
	ReplicationFunctionMap.Add( { GetRightArmPartAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_RightArmPart) } );
	ReplicationFunctionMap.Add( { GetLeftLegPartAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_LeftLegPart) } );
	ReplicationFunctionMap.Add( { GetRightLegPartAttribute(), FOnAttributeChangedDelegate::CreateUObject(this, &ThisClass::OnRep_RightLegPart) } );
}
