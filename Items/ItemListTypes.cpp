// All copyrights for this code are owned by Aster.


#include "ItemListTypes.h"
#include "ItemBase.h"
#include "ProjectPA/PALog.h"

// =============================================
//	Begin Encoded Item Information Version
// =============================================
void FEncodedItemList::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	if (OnItemRemoved.IsBound() == false)
	{
		return;
	}
	
	for (const int32 Index : RemovedIndices)
	{
		OnItemRemoved.Execute(Entries[Index].GetItemEncodedInfo());	
	}
}

void FEncodedItemList::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	if (OnItemAdded.IsBound() == false)
	{
		return;
	}
	
	for (const int32 Index : AddedIndices)
	{
		OnItemAdded.Execute(Entries[Index].GetItemEncodedInfo());
	}
}

void FEncodedItemList::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	for (const int32 Index : ChangedIndices)
	{
	}

	if (OnItemListChanged.IsBound())
	{
		OnItemListChanged.Execute();
	}
}

void FEncodedItemList::AddEntry(const UWorld* World, const FName& InItemKey, const FGameTimeInfo& InMadeDate, const float InRemainPercentage)
{
	if (World == nullptr)
	{
		return;
	}

	FEncodedItemListEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.ItemEncodedInfo = FItemEncodedInfo(InItemKey, InMadeDate, InRemainPercentage);

	MarkArrayDirty(); 
	MarkItemDirty(NewEntry);
}

void FEncodedItemList::AddEntry(const UWorld* World, const FItemEncodedInfo& ItemEncodedInfo)
{
	if (World == nullptr)
	{
		return;
	}

	FEncodedItemListEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.ItemEncodedInfo = ItemEncodedInfo;

	MarkArrayDirty(); 
	MarkItemDirty(NewEntry);
}

void FEncodedItemList::RemoveEntry(const FItemEncodedInfo& InItemEncodedInfo)
{
	for (auto It = Entries.CreateIterator(); It; ++It)
	{
		FEncodedItemListEntry& Entry = *It;
		if (Entry.ItemEncodedInfo != InItemEncodedInfo)
		{
			continue;
		}
		It.RemoveCurrent();
	}
	
	MarkArrayDirty();
}

bool FEncodedItemList::HasEntry(const FItemEncodedInfo& InItemEncodedInfo)
{
	for (auto It = Entries.CreateIterator(); It; ++It)
	{
		FEncodedItemListEntry& Entry = *It;
		if (Entry.ItemEncodedInfo == InItemEncodedInfo)
		{
			return true;
		}
	}
	return false;
}

void FEncodedItemList::AppendItemsToArray(TArray<FEncodedItemListEntry>& OutArray) const
{
	for (const FEncodedItemListEntry& Entry : Entries)
	{
		OutArray.Emplace(Entry);
	}
}

FItemEncodedInfo* FEncodedItemList::GetEntry(const FItemEncodedInfo& InItemEncodedInfo)
{
	for (auto It = Entries.CreateIterator(); It; ++It)
	{
		FEncodedItemListEntry& Entry = *It;
		if (Entry.ItemEncodedInfo == InItemEncodedInfo)
		{
			return &Entry.ItemEncodedInfo;
		}
	}

	return nullptr;
}
