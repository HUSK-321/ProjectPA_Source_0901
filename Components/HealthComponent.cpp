#include "HealthComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "ProjectPA/Characters/GameCharacterBase.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner())
	{
		OwnerCharacter = Cast<AGameCharacterBase>(GetOwner());
	}
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::OnHealthChanged(const float MaxHealth, const float CurHealth)
{
	OnHealthChangedEvent.Broadcast(MaxHealth, CurHealth);
}

void UHealthComponent::StartDying()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->StartDying();
		
		if(OwnerCharacter->GetController())
		{
			if (const AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController()))
			{
				if (AIController->GetBrainComponent())
				{
					AIController->GetBrainComponent()->StopLogic(TEXT("StartSying"));
				}
			}
		}
	}
}

void UHealthComponent::StartRest()
{
}

void UHealthComponent::StartSleep()
{
}


void UHealthComponent::OnHealthAttributeChanged(const float OldValue, const float NewValue)
{
	if (NewValue <= 0.0f)
	{
		StartDying();
	}
}

void UHealthComponent::OnMaxHealthAttributeChanged(const float OldValue, const float NewValue)
{
}

void UHealthComponent::OnMoveSpeedAttributeChanged(const float OldValue, const float NewValue)
{
}
