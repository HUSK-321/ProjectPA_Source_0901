#include "PlayGameHud.h"
#include "Blueprint/UserWidget.h"
#include "ProjectPA/Components/PlayerAbilitySystemComponent.h"
#include "ProjectPA/GameStates/PlayGameState.h"
#include "ProjectPA/GameStates/WeatherSystemComponent.h"
#include "ProjectPA/Generals/PAUtils.h"
#include "ProjectPA/Widgets/Informations/AnnounceMessageWidget.h"
#include "ProjectPA/Widgets/Informations/EnvironmentStatusWidget.h"
#include "ProjectPA/Widgets/Player/PlayerStatusWidget.h"
#include "ProjectPA/Widgets/Player/Item/PlayerInventoryWidget.h"
#include "ProjectPA/Widgets/Player/PlayerWidget.h"
#include "ProjectPA/Widgets/Player/StatusEffectListWidget.h"

APlayGameHud::APlayGameHud()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AnnounceMessageWidgetClass = UPAUtils::FindWidgetClass<UAnnounceMessageWidget>(TEXT("/Game/07_Widgets/Informations/WBP_AnnounceMessageWidget.WBP_AnnounceMessageWidget_C"));
	PlayerWidgetClass = UPAUtils::FindWidgetClass<UPlayerWidget>(TEXT("/Game/07_Widgets/WBP_PlayerHUD.WBP_PlayerHUD_C"));
}

void APlayGameHud::BeginPlay()
{
	Super::BeginPlay();

	if (AnnounceMessageWidgetClass)
	{
		AnnounceMessageWidget = CreateWidget<UAnnounceMessageWidget>(GetOwningPlayerController(), AnnounceMessageWidgetClass);
		if (AnnounceMessageWidget)
		{
			AnnounceMessageWidget->SetVisibility(ESlateVisibility::Hidden);
			AnnounceMessageWidget->AddToViewport();
		}
	}
	
	if (PlayerWidgetClass)
	{
		PlayerWidget = CreateWidget<UPlayerWidget>(GetOwningPlayerController(), PlayerWidgetClass);
		if (PlayerWidget)
		{
			PlayerWidget->SetVisibility(ESlateVisibility::Visible);
			PlayerWidget->AddToViewport();
			StatusWidget = PlayerWidget->GetPlayerStatusWidget();
			EnvironmentStatusWidget = PlayerWidget->GetEnvironmentStatusWidget();
		}
	}

	if (PlayerInventoryWidgetClass)
	{
		PlayerInventoryWidget = CreateWidget<UPlayerInventoryWidget>(GetOwningPlayerController(), PlayerInventoryWidgetClass);
		if (PlayerInventoryWidget)
		{
			PlayerInventoryWidget->AddToViewport();
			PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (IsValid(PlayerWidget) && IsValid(PlayerWidget->GetStatusEffectListWidget()) && IsValid(GetOwningPawn()))
	{
		UPlayerAbilitySystemComponent* PlayerASC = GetOwningPawn()->GetComponentByClass<UPlayerAbilitySystemComponent>();
		if (IsValid(PlayerASC))
		{
			PlayerASC->OnStatusEffectsUpdated.AddUObject(PlayerWidget->GetStatusEffectListWidget(), &UStatusEffectListWidget::OnStatusEffectListChanged);
		}
	}

	APlayGameState* GameState = Cast<APlayGameState>(GetWorld()->GetGameState());
	if (IsValid(EnvironmentStatusWidget) && IsValid(GameState))
	{
		EnvironmentStatusWidget->OnGameTimeChanged(GameState->GetGameTimeInfo());
		GameState->OnGameTimeChanged.AddUObject(EnvironmentStatusWidget, &UEnvironmentStatusWidget::OnGameTimeChanged);

		if (UWeatherSystemComponent* WeatherSystemComponent = GameState->GetComponentByClass<UWeatherSystemComponent>())
		{
			WeatherSystemComponent->OnTemperatureChanged.AddUObject(EnvironmentStatusWidget, &UEnvironmentStatusWidget::OnTemperatureChanged);
		}
	}
}

void APlayGameHud::ToggleInventoryWidget()
{
	if (PlayerInventoryWidget == nullptr)
	{
		return;
	}
	
	const bool bWillBeVisible = !PlayerInventoryWidget->IsVisible();
	PlayerInventoryWidget->SetVisibility(bWillBeVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		if (bWillBeVisible)
		{
			FInputModeGameAndUI InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}
		else
		{
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(false);
		}
	}
}

void APlayGameHud::SetAnnounceMessage(const FString& InMessage)
{
	if (AnnounceMessageWidget)
	{
		AnnounceMessageWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		AnnounceMessageWidget->AddAnnounce(InMessage);
	}
}
