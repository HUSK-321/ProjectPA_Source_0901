#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Interfaces/ItemContainer.h"
#include "ProjectPA/Interfaces/PersonalityTraitInteractable.h"
#include "NPCBase.generated.h"

class UHealthComponent;
class UBehaviorTree;
class UCharacterAttributeSet;

// TODO : 추후 IBTControllable -> GAS 이전 예정
UCLASS()
class PROJECTPA_API ANPCBase : public AGameCharacterBase, public IPersonalityTraitInteractable, public IItemContainer
{
	GENERATED_BODY()

private:
	FSimpleMulticastDelegate OnItemChangedEvent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCharacterAttributeSet> CharacterAttributeSet;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;

#if WITH_EDITORONLY_DATA
	/** 에디터용 테스트 아이템 Key 들입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, meta=(AllowPrivateAccess=true))
	TArray<FName> LootingItemKeysToSpawn;
#endif
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_LootingItemList, Category=Item, meta=(AllowPrivateAccess=true))
	FEncodedItemList LootingItemList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item, meta=(AllowPrivateAccess=true))
	FText ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Personality, meta=(AllowPrivateAccess=true))
	FActionWeightsArray ActionWeights;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Personality, meta=(AllowPrivateAccess=true))
	FPersonalityTraits Personality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Personality, meta=(AllowPrivateAccess=true))
	FToPlayerRelationships ToPlayerRelationships;
	
public:
	ANPCBase();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void StopAllActions() override;
	virtual void StartDying() override;
	virtual void DyingEnd() override;

	//~Begin IItemContainer Functions
	virtual const FText& GetItemActorName() const override { return ActorName; }
	
	virtual FDelegateHandle AddItemChangedDelegate(FSimpleDelegate& InDelegate) override { return OnItemChangedEvent.Add(InDelegate); }
	virtual void RemoveItemChangedDelegate(FDelegateHandle& InDelegateHandle) override { OnItemChangedEvent.Remove(InDelegateHandle); }
	
	virtual void SetOverlayEffect(const bool bEnable) override;
	
	virtual bool CanGetItemList() const override;
	virtual void GetItemList(OUT TArray<FItemEncodedInfo>& OutItemEncodedInfos) override;
	virtual FItemEncodedInfo* GetItem(const FItemEncodedInfo& ItemEncodedInfo) override;
	virtual bool HasItem(const FItemEncodedInfo& ItemEncodedInfo) override;
	virtual void AddItem(const FItemEncodedInfo& ItemEncodedInfo) override;
	virtual void RemoveItem(const FItemEncodedInfo& ItemEncodedInfo) override;
	
	virtual int32 GetContainerUID() const override { return GetUID(); }
	//~End IItemContainer Functions

	virtual const FItemEncodedInfo* FindBestItem(const EItemType ItemType, EItemComparisonLogic Comparator) const override;

	//~Begin IPersonalityTraitInteractable Functions
	virtual FToPlayerRelationships& GetToPlayerRelationships() override { return ToPlayerRelationships; }
	virtual int32 GetActionTypeWeight(EPersonalityActionType ActionType) override { return ActionWeights[ActionType]; }
	virtual void ApplyAction(AGameCharacterBase* Causer, EPlayerAction ActionType) override;
	//~End IPersonalityTraitInteractable Functions
	
protected:
	virtual void BeginPlay() override;
	virtual void OnAbilitySystemReady() override;

private:
	UFUNCTION()
	void OnRep_LootingItemList();
};
