#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectPA/Generals/PADelegates.h"
#include "HealthComponent.generated.h"

class UCharacterAttributeSet;
class UPlayerStatusWidget;
class AGameCharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTPA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FStatusChangedEvent OnHealthChangedEvent;
	
	FAttributeChangedEvent OnHealthAttributeChangedEvent;
	FAttributeChangedEvent OnMaxHealthAttributeChangedEvent;
	FAttributeChangedEvent OnMoveSpeedAttributeChangedEvent;
	
protected:
	UPROPERTY()
	TObjectPtr<AGameCharacterBase> OwnerCharacter;
	
public:
	UHealthComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnHealthChanged(const float MaxHealth, const float CurHealth);
	
	virtual void OnHealthAttributeChanged(const float OldValue, const float NewValue);
	virtual void OnMaxHealthAttributeChanged(const float OldValue, const float NewValue);
	virtual void OnMoveSpeedAttributeChanged(const float OldValue, const float NewValue);
	
	virtual void StartDying();
	virtual void StartRest();
	virtual void StartSleep();

protected:
	virtual void BeginPlay() override;

	FORCEINLINE AGameCharacterBase* GetOwnerCharacter() const { return OwnerCharacter; }
};
