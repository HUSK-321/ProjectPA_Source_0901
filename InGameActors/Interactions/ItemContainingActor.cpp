// All copyrights for this code are owned by Aster.
#include "ItemContainingActor.h"
#include "Net/UnrealNetwork.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"
#include "ProjectPA/Items/ItemBase.h"


AItemContainingActor::AItemContainingActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void AItemContainingActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemContainingActor, ItemKeyList);
}

void AItemContainingActor::BeginPlay()
{
	Super::BeginPlay();

	ItemKeyList.OnItemListChanged.BindUObject(this, &AItemContainingActor::OnRep_ItemKeyList);

#if WITH_EDITORONLY_DATA
	for (const FName& ItemKeyToSpawn : ItemKeysToSpawnOnBeginPlay)
	{
		ItemKeyList.AddEntry(GetWorld(), ItemKeyToSpawn);
	}
#endif
}

bool AItemContainingActor::CanGetItemList() const
{
	return !ItemKeyList.IsEmpty();
}

void AItemContainingActor::GetItemList(TArray<FItemEncodedInfo>& OutItemEncodedInfos)
{
	for (auto Entry : ItemKeyList.GetEntries())
	{
		OutItemEncodedInfos.Emplace(Entry.GetItemEncodedInfo());
	}
}

void AItemContainingActor::SetOverlayEffect(const bool bEnable)
{
	check(Mesh);
	if (CanGetItemList() == false)
	{
		return;
	}
	
	Mesh->SetOverlayMaterialMaxDrawDistance(bEnable ? 0.f : -1.f);
}

FItemEncodedInfo* AItemContainingActor::GetItem(const FItemEncodedInfo& ItemEncodedInfo)
{
	return ItemKeyList.GetEntry(ItemEncodedInfo);
}

bool AItemContainingActor::HasItem(const FItemEncodedInfo& ItemEncodedInfo)
{
	return ItemKeyList.HasEntry(ItemEncodedInfo);
}

void AItemContainingActor::AddItem(const FItemEncodedInfo& ItemEncodedInfo)
{
	ItemKeyList.AddEntry(GetWorld(), ItemEncodedInfo);
	ItemKeyList.MarkArrayDirty();
	OnRep_ItemKeyList();
}

void AItemContainingActor::RemoveItem(const FItemEncodedInfo& ItemEncodedInfo)
{
	ItemKeyList.RemoveEntry(ItemEncodedInfo);
	ItemKeyList.MarkArrayDirty();
	OnRep_ItemKeyList();
}

void AItemContainingActor::OnRep_ItemKeyList()
{
	OnItemChangedEvent.Broadcast();
}
