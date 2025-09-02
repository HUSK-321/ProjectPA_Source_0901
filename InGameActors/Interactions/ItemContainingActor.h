// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/InGameActors/PAActorBase.h"
#include "ProjectPA/Interfaces/ItemContainer.h"
#include "ProjectPA/Items/ItemListTypes.h"
#include "ItemContainingActor.generated.h"

UCLASS()
class PROJECTPA_API AItemContainingActor : public APAActorBase, public IItemContainer
{
	GENERATED_BODY()

private:
	FSimpleMulticastDelegate OnItemChangedEvent;
	
protected:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(AllowPrivateAccess=true))
	FText ContainerName;
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Replicated)
	TArray<FName> ItemKeysToSpawnOnBeginPlay;
#endif
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_ItemKeyList)
	FEncodedItemList ItemKeyList;

public:
	AItemContainingActor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//~Begin IItemContainer Functions
	virtual const FText& GetItemActorName() const override { return ContainerName; }
	
	virtual FDelegateHandle AddItemChangedDelegate(FSimpleDelegate& InDelegate) override { return OnItemChangedEvent.Add(InDelegate); }
	virtual void RemoveItemChangedDelegate(FDelegateHandle& InDelegateHandle) override { OnItemChangedEvent.Remove(InDelegateHandle); }
	
	virtual void SetOverlayEffect(const bool bEnable) override;

	virtual bool CanGetItemList() const override;
	virtual void GetItemList(TArray<FItemEncodedInfo>& OutItemEncodedInfos) override;
	virtual FItemEncodedInfo* GetItem(const FItemEncodedInfo& ItemEncodedInfo) override;
	virtual bool HasItem(const FItemEncodedInfo& ItemEncodedInfo) override;
	virtual void AddItem(const FItemEncodedInfo& ItemEncodedInfo) override;
	virtual void RemoveItem(const FItemEncodedInfo& ItemEncodedInfo) override;

	virtual int32 GetContainerUID() const override { return GetUID(); }
	//~End IItemContainer Functions
	
protected:
	virtual void BeginPlay() override;
	
	void SpawnItemForInit();

private:
	UFUNCTION()
	void OnRep_ItemKeyList();

	UItemBase* GetSpawnedItemByInfo(const FItemEncodedInfo& Info);
};
