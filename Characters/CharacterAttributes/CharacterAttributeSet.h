// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ProjectPA/Generals/PADelegates.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \

#define ATTRIBUTE_WITH_EVENT(ClassName, PropertyName) \
FAttributeChangedEvent PropertyName##AttributeChangedEvent; \
ATTRIBUTE_ACCESSORS(ClassName, PropertyName)

DECLARE_DELEGATE_OneParam(FOnAttributeChangedDelegate, const FGameplayAttributeData&);

/**
 * 
 */
UCLASS()
class PROJECTPA_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	FStatusChangedEvent OnHealthChangedEvent;

	FBodyPartChanged OnBodyPartChanged;

protected:
	// 체력 (Health)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health, meta=(AllowPrivateAccess))
	FGameplayAttributeData Health;
	// 최대 체력 (MaxHealth)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth, meta=(AllowPrivateAccess))
	FGameplayAttributeData MaxHealth;
	// 이동 속도 (MoveSpeed)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MoveSpeed, meta=(AllowPrivateAccess))
	FGameplayAttributeData MoveSpeed;
	// 최대 달리기 속도 (SprintSpeed)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_SprintSpeed, meta=(AllowPrivateAccess))
	FGameplayAttributeData SprintSpeed;
	// 앉아서 이동하는 속도 (CrouchSpeed)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_CrouchSpeed, meta=(AllowPrivateAccess))
	FGameplayAttributeData CrouchSpeed;
	// 현재 속도에 적용될 비율
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_SpeedRatio, meta=(AllowPrivateAccess))
	FGameplayAttributeData SpeedRatio;
	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_Sprinting, meta=(AllowPrivateAccess))
	bool bCharacterSprinting;
	UPROPERTY(BlueprintReadOnly, Category = "Status", ReplicatedUsing = OnRep_Crouching, meta=(AllowPrivateAccess))
	bool bCharacterCrouching;

	// 플레이어의 부위
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_HeadPart, meta=(AllowPrivateAccess))
	FGameplayAttributeData HeadPart;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_BodyPart, meta=(AllowPrivateAccess))
	FGameplayAttributeData BodyPart;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_LeftArmPart, meta=(AllowPrivateAccess))
	FGameplayAttributeData LeftArmPart;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_RightArmPart, meta=(AllowPrivateAccess))
	FGameplayAttributeData RightArmPart;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_LeftLegPart, meta=(AllowPrivateAccess))
	FGameplayAttributeData LeftLegPart;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_RightLegPart, meta=(AllowPrivateAccess))
	FGameplayAttributeData RightLegPart;

	/** Only for listen-server or standalone */
	TMap<FGameplayAttribute, FOnAttributeChangedDelegate> ReplicationFunctionMap;

public:
	UCharacterAttributeSet(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BindHealthComponentEnd();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	ATTRIBUTE_WITH_EVENT(UCharacterAttributeSet, Health);
	ATTRIBUTE_WITH_EVENT(UCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_WITH_EVENT(UCharacterAttributeSet, MoveSpeed);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, SprintSpeed);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, CrouchSpeed);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, SpeedRatio);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HeadPart);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, BodyPart);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, LeftArmPart);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, RightArmPart);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, LeftLegPart);
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, RightLegPart);

	void SetSprinting(const bool bSprinting);
	void SetCrouching(const bool bCrouching);
	FORCEINLINE bool IsSprinting() const { return bCharacterSprinting; }
	
protected:
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
	UFUNCTION()
	virtual void OnRep_SprintSpeed(const FGameplayAttributeData& OldSprintSpeed);
	UFUNCTION()
	virtual void OnRep_CrouchSpeed(const FGameplayAttributeData& OldCrouchSpeed);
	UFUNCTION()
	virtual void OnRep_SpeedRatio(const FGameplayAttributeData& OldSpeedRatio);

	UFUNCTION()
	virtual void OnRep_HeadPart(const FGameplayAttributeData& OldHeadPart);
	UFUNCTION()
	virtual void OnRep_BodyPart(const FGameplayAttributeData& OldBodyPart);
	UFUNCTION()
	virtual void OnRep_LeftArmPart(const FGameplayAttributeData& OldLeftArmPart);
	UFUNCTION()
	virtual void OnRep_RightArmPart(const FGameplayAttributeData& OldRightArmPart);
	UFUNCTION()
	virtual void OnRep_LeftLegPart(const FGameplayAttributeData& OldLeftLegPart);
	UFUNCTION()
	virtual void OnRep_RightLegPart(const FGameplayAttributeData& OldRightLegPart);
	
	UFUNCTION()
	virtual void OnRep_Sprinting();
	UFUNCTION()
	virtual void OnRep_Crouching();
	
	void SetOwnerCharacterMoveSpeed();
	void SetOwnerCharacterCrouch();

	float GetMaxMoveSpeedByStatus();
};
