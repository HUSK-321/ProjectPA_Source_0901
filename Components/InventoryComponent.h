// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ProjectPA/Generals/PATypes.h"
#include "InventoryComponent.generated.h"

class IItemContainer;
class UBagItemBase;
enum class EItemType : uint8;
enum class EGetItemError : uint8;
class UItemBase;
class UPlayerInventoryWidget;
class AItemActorBase;
class AGameCharacterBase;

DECLARE_EVENT(UInventoryComponent, FOnPlayerItemListChanged)
DECLARE_EVENT_OneParam(UInventoryComponent, FOnWeightChanged, const float /* NewWeight */);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTPA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnPlayerItemListChanged OnPlayerItemListChanged;
	FOnWeightChanged OnWeightChanged;
	
private:
	/** 아이템을 바로 사용할 수 있도록 하는 어빌리티의 Tag. EquipmentComponent 를 통하지 않고 아이템을 사용할 때 사용합니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FGameplayTag UseItemDirectlyTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 MaxBagCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<UBagItemBase*> Bags;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float CurrentWeight;
	int32 CurrentBagIndex;
	
	UPROPERTY()
	TObjectPtr<AGameCharacterBase> CachedOwnerCharacter;
	
public:
	UInventoryComponent();

	FItemEncodedInfo* GetItem(const FItemEncodedInfo& InItemEncodedInfo);
	void TryGetItem(const FItemEncodedInfo& ItemToWant, int32 OriginItemOwnerUID);
	UFUNCTION(Reliable, Server)
	void ServerGetItem(int32 ClientBagIndex, const FItemEncodedInfo& ItemToWant, int32 OriginItemOwnerUID);
	bool RemoveItem(const FItemEncodedInfo& ItemToRemove);
	bool RemoveItem(int8 BagIndex, const FItemEncodedInfo& ItemEncodedInfo);
	void UseItem(const FItemEncodedInfo& ItemEncodedInfo, int32 OriginOwnerUID);
	UFUNCTION(Server, Reliable)
	void ServerUseItem(const FItemEncodedInfo& ItemEncodedInfo, const int32 OriginOwnerUID);
	
	/// @param ItemEncodedInfo Item to find 
	/// @return Return item's bag index. Return INDEX_NONE when don't have. 
	int32 HaveItem(const FItemEncodedInfo& ItemEncodedInfo);
	bool HaveItem(const FName& ItemKeyToFind);
	
	const TArray<UBagItemBase*>& GetPlayerBags() const { return Bags; }
	float GetCurrentWeight() const { return CurrentWeight; }
	void CalculateWeight();

protected:	
	virtual void BeginPlay() override;

private:
};
