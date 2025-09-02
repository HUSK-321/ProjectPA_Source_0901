// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ProjectPA/Generals/PATypes.h"
#include "ItemListTypes.generated.h"

class UItemBase;
struct FItemList;

// =============================================
//	Begin Encoded Item Information Version
// =============================================

DECLARE_DELEGATE_OneParam(FOnItemChanged, const FItemEncodedInfo&);

USTRUCT(BlueprintType)
struct PROJECTPA_API FEncodedItemListEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	FEncodedItemListEntry()
	{}

	const FItemEncodedInfo& GetItemEncodedInfo() const { return ItemEncodedInfo; }

private:
	friend FEncodedItemList;

	UPROPERTY(EditAnywhere)
	FItemEncodedInfo ItemEncodedInfo;
};

USTRUCT(BlueprintType)
struct PROJECTPA_API FEncodedItemList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FSimpleDelegate OnItemListChanged;
	FOnItemChanged OnItemAdded;
	FOnItemChanged OnItemRemoved;
	
	FEncodedItemList()
		:	OwnerActor(nullptr)
	{}

	FEncodedItemList(AActor* InOwnerActor)
		: OwnerActor(InOwnerActor)
	{}

	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FEncodedItemListEntry, FEncodedItemList>(Entries, DeltaParms, *this);
	}

	void AddEntry(const UWorld* World, const FName& InItemKey, const FGameTimeInfo& InMadeDate = GameStartTime, const float InRemainPercentage = 1.0f);
	void AddEntry(const UWorld* World, const FItemEncodedInfo& ItemEncodedInfo);
	void RemoveEntry(const FItemEncodedInfo& InItemEncodedInfo);

	bool HasEntry(const FItemEncodedInfo& InItemEncodedInfo);
	
	void AppendItemsToArray(TArray<FEncodedItemListEntry>& OutArray) const;
	bool IsEmpty() const { return Entries.IsEmpty(); }

	const TArray<FEncodedItemListEntry>& GetEntries() const { return Entries; }
	FItemEncodedInfo* GetEntry(const FItemEncodedInfo& InItemEncodedInfo);
	
private:
	// Replicated list
	UPROPERTY(EditAnywhere)
	TArray<FEncodedItemListEntry> Entries;
	UPROPERTY(NotReplicated)
	TObjectPtr<AActor> OwnerActor;
};

template<>
struct TStructOpsTypeTraits<FEncodedItemList> : public TStructOpsTypeTraitsBase2<FEncodedItemList>
{
	enum { WithNetDeltaSerializer = true };
};
