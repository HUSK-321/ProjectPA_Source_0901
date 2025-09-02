#include "NPCBase.h"
#include "AIController.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"
#include "ProjectPA/Abilities/AbilityObjects/ItemComparisonRules.h"
#include "ProjectPA/Characters/CharacterAttributes/CharacterAttributeSet.h"
#include "ProjectPA/Components/CharacterAbilitySystemComponent.h"
#include "ProjectPA/Components/EquipmentComponent.h"
#include "ProjectPA/Components/HealthComponent.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"
#include "ProjectPA/Items/ItemBase.h"
#include "ProjectPA/Items/ItemDataTables.h"
#include "ProjectPA/Items/WeaponFires/WeaponFireBase.h"

ANPCBase::ANPCBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("Character Attribute Set"));

	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilityComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

void ANPCBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANPCBase, LootingItemList);
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITORONLY_DATA
	if (HasAuthority() && GetWorld())
	{
		for (const FName& ItemKey : LootingItemKeysToSpawn)
		{
			LootingItemList.AddEntry(GetWorld(), ItemKey);
		}
	}
#endif
	
	SetOverlayEffect(false);
}

void ANPCBase::OnAbilitySystemReady()
{
	Super::OnAbilitySystemReady();

	if (HealthComponent && CharacterAttributeSet)
	{
		CharacterAttributeSet->HealthAttributeChangedEvent.AddUObject(HealthComponent, &UHealthComponent::OnHealthAttributeChanged);
		CharacterAttributeSet->MaxHealthAttributeChangedEvent.AddUObject(HealthComponent, &UHealthComponent::OnMaxHealthAttributeChanged);
		CharacterAttributeSet->MoveSpeedAttributeChangedEvent.AddUObject(HealthComponent, &UHealthComponent::OnMoveSpeedAttributeChanged);
	}

	CharacterAttributeSet->BindHealthComponentEnd();
}

void ANPCBase::StartDying()
{
	Super::StartDying();

	FTimerHandle RagDolTimer;
	GetWorldTimerManager().SetTimer(RagDolTimer, this, &ANPCBase::DyingEnd, 2.f);
}

void ANPCBase::DyingEnd()
{
	Super::DyingEnd();
}

void ANPCBase::SetOverlayEffect(const bool bEnable)
{
	check(GetMesh());
	GetMesh()->SetOverlayMaterialMaxDrawDistance(bEnable ? 0.f : -1.f);
}

//~Begin IItemContainer Functions
bool ANPCBase::CanGetItemList() const
{
	return !LootingItemList.IsEmpty() && bCharacterDeadEnd;
}

void ANPCBase::GetItemList(TArray<FItemEncodedInfo>& OutItemEncodedInfos)
{
	for (const auto& Entry : LootingItemList.GetEntries())
	{
		OutItemEncodedInfos.Emplace(Entry.GetItemEncodedInfo());
	}
}

FItemEncodedInfo* ANPCBase::GetItem(const FItemEncodedInfo& ItemEncodedInfo)
{
	return LootingItemList.GetEntry(ItemEncodedInfo);
}

bool ANPCBase::HasItem(const FItemEncodedInfo& ItemEncodedInfo)
{
	return LootingItemList.HasEntry(ItemEncodedInfo);
}

void ANPCBase::AddItem(const FItemEncodedInfo& ItemEncodedInfo)
{
	if (HasAuthority())
	{
		LootingItemList.AddEntry(GetWorld(), ItemEncodedInfo);
		OnItemChangedEvent.Broadcast();
	}
}

void ANPCBase::RemoveItem(const FItemEncodedInfo& ItemEncodedInfo)
{
	if (HasAuthority())
	{
		LootingItemList.RemoveEntry(ItemEncodedInfo);
		OnItemChangedEvent.Broadcast();
	}
}

//~End IItemContainer Functions

const FItemEncodedInfo* ANPCBase::FindBestItem(const EItemType ItemType, EItemComparisonLogic ComparatorLogic) const
{
	const FItemEncodedInfo* BestItemInfo = nullptr;
	const FItemDataTable* BestItemData = nullptr;

	if (const IGameDataTableProvider* DataProvider = GetWorld()->GetGameInstance<IGameDataTableProvider>())
	{
		for (const FEncodedItemListEntry& Entry : LootingItemList.GetEntries())
		{
			const FItemEncodedInfo& CurrentItemInfo = Entry.GetItemEncodedInfo();
			const FItemDataTable* CurrentItemData = DataProvider->GetItemDataByKey(CurrentItemInfo.GetItemKey());

			if (CurrentItemData && CurrentItemData->ItemType == ItemType)
			{
				if (BestItemInfo == nullptr || ItemComparator::Compare(ComparatorLogic, CurrentItemInfo, CurrentItemData, *BestItemInfo, BestItemData))
				{
					BestItemInfo = &CurrentItemInfo;
					BestItemData = CurrentItemData;
				}
			}
		}
	}

	return BestItemInfo;
}

void ANPCBase::ApplyAction(AGameCharacterBase* Causer, EPlayerAction ActionType)
{
	
}

void ANPCBase::StopAllActions()
{
	Super::StopAllActions();

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UAIPerceptionComponent* PerceptionComp = AIController->FindComponentByClass<UAIPerceptionComponent>())
		{
			PerceptionComp->OnPerceptionUpdated.Clear();
			PerceptionComp->OnTargetPerceptionUpdated.Clear();
			PerceptionComp->OnTargetPerceptionInfoUpdated.Clear();
			PerceptionComp->SetActive(false);
		}

		AIController->UnPossess();
		UnPossessed();
		AIController->Destroy();
	}
}

void ANPCBase::OnRep_LootingItemList()
{
	OnItemChangedEvent.Broadcast();
}
