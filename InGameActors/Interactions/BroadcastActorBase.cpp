// All copyrights for this code are owned by Aster.

#include "BroadcastActorBase.h"
#include "Components/WidgetComponent.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Data/Dialogue/BroadcastTimeDataTable.h"
#include "ProjectPA/GameStates/PlayGameState.h"
#include "ProjectPA/Widgets/Informations/BroadcastActorWidget.h"

ABroadcastActorBase::ABroadcastActorBase()
	: CurrentBroadcast(nullptr)
	, BroadcastInterval(2.0f)
	, CurrentDialogueIndex(0)
{
	PrimaryActorTick.bCanEverTick = false;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	SetRootComponent(ActorMesh);

	BroadcastWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BroadcastWidgetComponent"));
	BroadcastWidgetComponent->SetupAttachment(GetRootComponent());
}

void ABroadcastActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	APlayGameState* GameState = Cast<APlayGameState>(GetWorld()->GetGameState());
	if (IsValid(GameState))
	{
		GetGameTimeDelegate.BindUObject(GameState, &APlayGameState::GetGameTimeInfo);
	}

	check(BroadcastWidgetComponent);
	BroadcastWidget = Cast<UBroadcastActorWidget>(BroadcastWidgetComponent->GetWidget());

	SetBroadcastWidget(false);
}

void ABroadcastActorBase::InteractWith(APlayerCharacter* InteractingPlayer)
{
	IInteractableActor::InteractWith(InteractingPlayer);
}

void ABroadcastActorBase::OnInteractStart(APlayerCharacter* InteractingPlayer)
{
	IInteractableActor::OnInteractStart(InteractingPlayer);

	if (IsBroadcastTime())
	{
		SetBroadcastWidget(true);
		DisplayBroadcast();
	}
}

void ABroadcastActorBase::OnInteractStop(APlayerCharacter* InteractingPlayer)
{
	SetBroadcastWidget(false);
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer( BroadcastTimer);
	}
	IInteractableActor::OnInteractStop(InteractingPlayer);
}

void ABroadcastActorBase::DisplayBroadcast()
{
	FindNewBroadcast();
	if (CurrentBroadcast == nullptr)
	{
		return;
	}

	const FName& DialogueKey = GetBroadcastDialogueKey();
	if (DialogueKey.IsNone() == false && BroadcastWidget)
	{
		if (const FBroadcastDataTable* BroadcastData = GetBroadcastData(DialogueKey))
		{
			BroadcastWidget->SetBroadcastText(BroadcastData->Broadcaster, BroadcastData->BroadcastText);	
		}
	}
	
	ReserveNextBroadcast();
}

void ABroadcastActorBase::ReserveNextBroadcast()
{
	if (CurrentBroadcast == nullptr || CurrentBroadcast->DialogueKeys.IsEmpty())
	{
		return;
	}
	
	if (GetWorld())
	{
		CurrentDialogueIndex += 1;
		if (CurrentBroadcast && CurrentBroadcast->DialogueKeys.Num() <= CurrentDialogueIndex)
		{
			CurrentDialogueIndex = 0;
		}
		
		GetWorld()->GetTimerManager().SetTimer(BroadcastTimer, this, &ABroadcastActorBase::DisplayBroadcast, BroadcastInterval);
	}
}

void ABroadcastActorBase::UpdateCurrentBroadcast()
{
	if (IsValid(BroadcastTimeDataTable) == false)
	{
		return;
	}
	
	const FGameTimeInfo& CurrentTime = GetGameTime();
	if (CurrentBroadcast != nullptr && CurrentBroadcast->IsTimeInBroadcastRange(CurrentTime))
	{
		return;
	}
	
	// 현재 방송이 끝났으면 정리
	CurrentBroadcast = nullptr;
	CurrentDialogueIndex = 0;

	// 2. 현재 방송이 없거나 끝났다면 새로운 방송 찾기
	FindNewBroadcast();
}

void ABroadcastActorBase::FindNewBroadcast()
{
	if (IsValid(BroadcastTimeDataTable) == false)
	{
		return;
	}
	
	const FGameTimeInfo& CurrentTime = GetGameTime();
	if (CurrentBroadcast != nullptr && CurrentBroadcast->IsTimeInBroadcastRange(CurrentTime))
	{
		return;
	}

	TArray<FBroadcastTimeDataTable*> AllBroadcasts;
	BroadcastTimeDataTable->GetAllRows<FBroadcastTimeDataTable>(TEXT("FindNewBroadcast"), AllBroadcasts);

	for (FBroadcastTimeDataTable* Broadcast : AllBroadcasts)
	{
		if (Broadcast && Broadcast->IsTimeInBroadcastRange(CurrentTime))
		{
			CurrentBroadcast = Broadcast;
			break;
		}
	}
    
	if (CurrentBroadcast)
	{
		CurrentDialogueIndex = 0;
	}
}

FName ABroadcastActorBase::GetBroadcastDialogueKey()
{
	if (CurrentBroadcast == nullptr)
	{
		return TEXT("None");
	}

	const TArray<FName>& DialogueKeys = CurrentBroadcast->DialogueKeys;
	if (DialogueKeys.IsEmpty())
	{
		return TEXT("None");
	}
	
	CurrentDialogueIndex = DialogueKeys.IsValidIndex(CurrentDialogueIndex) ? CurrentDialogueIndex : 0;
	return DialogueKeys[CurrentDialogueIndex];
}

const FBroadcastDataTable* ABroadcastActorBase::GetBroadcastData(const FName& BroadcastKey) const
{
	if (BroadcastDataTable)
	{
		return BroadcastDataTable->FindRow<FBroadcastDataTable>(BroadcastKey, TEXT("GetBroadcastData"));
	}
	return nullptr;
}

const FGameTimeInfo& ABroadcastActorBase::GetGameTime()
{
	// 1. Use cached delegate function.
	if (GetGameTimeDelegate.IsBound())
	{
		return GetGameTimeDelegate.Execute();
	}

	// 2. If there's no cached function, get game state and cache function.
	if (GetWorld())
	{
		APlayGameState* GameState = Cast<APlayGameState>(GetWorld()->GetGameState());
		if (IsValid(GameState))
		{
			GetGameTimeDelegate.BindUObject(GameState, &APlayGameState::GetGameTimeInfo);
			return GameState->GetGameTimeInfo();
		}
	}

	if (GetWorld())
	{
		PALog_E(TEXT("There is no valid game state : %s"), *GetNameSafe(GetWorld()->GetGameState()));
	}
	else
	{
		PALog_E(TEXT("There is no valid world"));
	}
	return INVALID_GAME_TIME;
}

bool ABroadcastActorBase::IsBroadcastTime()
{
	FindNewBroadcast();
	return CurrentBroadcast != nullptr && CurrentBroadcast->IsTimeInBroadcastRange(GetGameTime());
}

void ABroadcastActorBase::SetBroadcastWidget(bool bVisible)
{
	if (BroadcastWidget)
	{
		BroadcastWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
