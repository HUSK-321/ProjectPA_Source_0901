// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAttributeSet.h"
#include "PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UPlayerAttributeSet : public UCharacterAttributeSet
{
	GENERATED_BODY()

public:
	mutable FAttributeChanged AttributeChangedEvent;
	
	FStatusChangedEvent OnStaminaChangedEvent;

	ATTRIBUTE_WITH_EVENT(UPlayerAttributeSet, Stamina);
	ATTRIBUTE_WITH_EVENT(UPlayerAttributeSet, MaxStamina);
	ATTRIBUTE_WITH_EVENT(UPlayerAttributeSet, Hunger);
	ATTRIBUTE_WITH_EVENT(UPlayerAttributeSet, Thirsty);
	ATTRIBUTE_WITH_EVENT(UPlayerAttributeSet, Stress);
	ATTRIBUTE_WITH_EVENT(UPlayerAttributeSet, Tired);
	ATTRIBUTE_WITH_EVENT(UPlayerAttributeSet, Sleep);
	ATTRIBUTE_WITH_EVENT(UPlayerAttributeSet, TemperatureDeviation);

private:
	// 스테미나 (Stamina)
	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes", ReplicatedUsing = OnRep_Stamina, meta=(AllowPrivateAccess))
	FGameplayAttributeData Stamina;
	// 최대 스테미나 (MaxStamina)
	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes", ReplicatedUsing = OnRep_MaxStamina, meta=(AllowPrivateAccess))
	FGameplayAttributeData MaxStamina;
	// 배고픔 (Hunger)
	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes", ReplicatedUsing = OnRep_Hunger, meta=(AllowPrivateAccess))
	FGameplayAttributeData Hunger;
	// 목마름 (Thirsty)
	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes", ReplicatedUsing = OnRep_Thirsty, meta=(AllowPrivateAccess))
	FGameplayAttributeData Thirsty;
	// 스트레스 (Stress)
	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes", ReplicatedUsing = OnRep_Stress, meta=(AllowPrivateAccess))
	FGameplayAttributeData Stress;
	// 피곤함 (Tired)
	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes", ReplicatedUsing = OnRep_Tired, meta=(AllowPrivateAccess))
	FGameplayAttributeData Tired;
	// 잠 (Sleep)
	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes", ReplicatedUsing = OnRep_Sleep, meta=(AllowPrivateAccess))
	FGameplayAttributeData Sleep;
	// 현재 보온이 적절 보온에서 얼마나 벗어났는지 (TemperatureDeviation)
	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes", ReplicatedUsing = OnRep_TemperatureDeviation, meta=(AllowPrivateAccess))
	FGameplayAttributeData TemperatureDeviation;

public:
	UPlayerAttributeSet(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void BindHealthComponentEnd() override;

	void OnPlayerMoved(const int32 MovedMeter);

private:
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;
	UFUNCTION()
	void OnRep_Hunger(const FGameplayAttributeData& OldHunger) const;
	UFUNCTION()
	void OnRep_Thirsty(const FGameplayAttributeData& OldThirsty) const;
	UFUNCTION()
	void OnRep_Stress(const FGameplayAttributeData& OldStress) const;
	UFUNCTION()
	void OnRep_Tired(const FGameplayAttributeData& OldTired) const;
	UFUNCTION()
	void OnRep_Sleep(const FGameplayAttributeData& OldSleep) const;
	UFUNCTION()
	void OnRep_TemperatureDeviation(const FGameplayAttributeData& OldTemperatureDeviation) const;
};
