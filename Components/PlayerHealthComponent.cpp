#include "PlayerHealthComponent.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/GameStates/PlayGameState.h"
#include "ProjectPA/HUD/PlayGameHud.h"
#include "ProjectPA/Widgets/Player/PlayerStatusWidget.h"

UPlayerHealthComponent::UPlayerHealthComponent()
{
}

void UPlayerHealthComponent::SetupHealthWidget()
{
	if (GetOwner())
	{
		OwnerCharacter = Cast<AGameCharacterBase>(GetOwner());
	}
	
	BindWidgetEvents();
}

void UPlayerHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(GetOwnerCharacter()) == false)
	{
		return;
	}
		
	const FVector CurLocation = GetOwnerCharacter()->GetActorLocation();
	PrevMoveDistance += (PrevLocation - CurLocation).Size2D();
	
	if (PrevMoveDistance > 1.0f)
	{
		const int32 MovedDistance = FMath::Floor(PrevMoveDistance);
		const int32 MovedMeter = MovedDistance / 100;
		if (MovedMeter > 0)
		{
			PrevMoveDistance -= MovedMeter * 100;
			OnPlayerMovedEvent.Broadcast(MovedMeter);
		}
	}
	PrevLocation = CurLocation;
}

void UPlayerHealthComponent::OnHealthChanged(const float MaxHealth, const float CurHealth)
{
	Super::OnHealthChanged(MaxHealth, CurHealth);
}

void UPlayerHealthComponent::BeginPlay()
{
	if (GetOwnerCharacter())
	{
		PrevLocation = GetOwnerCharacter()->GetActorLocation();
	}
	
	Super::BeginPlay();

	if (APlayGameState* GameState = GetWorld()->GetGameState<APlayGameState>())
	{
		GameState->OnGameTimeChanged.AddUObject(this, &UPlayerHealthComponent::OnGameTimeChanged);
	}
}

void UPlayerHealthComponent::BindWidgetEvents()
{
	if (GetOwnerCharacter() && GetOwnerCharacter()->GetController())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(GetOwnerCharacter()->GetController()))
		{
			APlayGameHud* HUD = PlayerController->GetHUD<APlayGameHud>();
			if (HUD)
			{
				OnHealthChangedEvent.AddUObject(HUD->GetStatusWidget(), &UPlayerStatusWidget::SetHealthBar);
				OnStaminaChangedEvent.AddUObject(HUD->GetStatusWidget(), &UPlayerStatusWidget::SetStaminaBar);
				OnBodyPartChangedEvent.AddUObject(HUD->GetStatusWidget(), &UPlayerStatusWidget::SetPartValue);
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UPlayerHealthComponent::BindWidgetEvents);
			}
		}
	}
}

void UPlayerHealthComponent::OnStaminaChanged(const float MaxStamina, const float CurStamina)
{
	OnStaminaChangedEvent.Broadcast(MaxStamina, CurStamina);
}

void UPlayerHealthComponent::OnHealthAttributeChanged(const float OldValue, const float NewValue)
{
	Super::OnHealthAttributeChanged(OldValue, NewValue);
	OnHealthAttributeChangedEvent.Broadcast(OldValue, NewValue);
}

void UPlayerHealthComponent::OnMaxHealthAttributeChanged(const float OldValue,
                                                         const float NewValue)
{
	Super::OnMaxHealthAttributeChanged(OldValue, NewValue);
	OnMaxHealthAttributeChangedEvent.Broadcast(OldValue, NewValue);
}

void UPlayerHealthComponent::SetPartValue(const EBodyPart InBodyPart, const float InCurrentValue,
	const float InMaxValue)
{
	OnBodyPartChangedEvent.Broadcast(InBodyPart, InCurrentValue, InMaxValue);
}

void UPlayerHealthComponent::StartDying()
{
	Super::StartDying();
}

void UPlayerHealthComponent::StartRest()
{
	Super::StartRest();
}

void UPlayerHealthComponent::StartSleep()
{
	Super::StartSleep();
}

void UPlayerHealthComponent::ToggleStatusPartWidget()
{
	if (GetOwnerCharacter() && GetOwnerCharacter()->GetController())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(GetOwnerCharacter()->GetController()))
		{
			APlayGameHud* HUD = PlayerController->GetHUD<APlayGameHud>();
			if (HUD && HUD->GetStatusWidget())
			{
				HUD->GetStatusWidget()->ToggleBodyPartWidget();
			}
		}
	}
}

void UPlayerHealthComponent::OnGameTimeChanged(const FGameTimeInfo& NewTime)
{
	// Info : 이 게임은 일단 실제 60초 기준으로 목마름/배고픔을 계산하기로 했다

	if (GameTimePeriodicEffects.Num() == 0)
	{
		return;
	}
	if (GetOwnerCharacter() == nullptr || GetOwnerCharacter()->GetAbilitySystemComponent() == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetOwnerCharacter()->GetAbilitySystemComponent();
	for (TSubclassOf<UGameplayEffect>& EffectToGrant : GameTimePeriodicEffects)
	{
		if (EffectToGrant == nullptr)
		{
			continue;
		}

		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectToGrant, 1.0f, ASC->MakeEffectContext());
		if (EffectSpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}
