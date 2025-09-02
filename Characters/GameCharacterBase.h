#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectPA/Data/Audio/FootstepAssetBank.h"
#include "ProjectPA/Generals/PATypes.h"
#include "ProjectPA/Interfaces/FootstepAssetProvider.h"
#include "ProjectPA/Interfaces/PAActor.h"
#include "AbilitySystemInterface.h"
#include "GameCharacterBase.generated.h"

enum class EItemComparisonLogic : uint8;
class UItemComparisonRuleBase;
class UEquipmentComponent;
class ISwimmableVolume;
class UItemBase;
class UCharacterAbilitySystemComponent;
class UMotionWarpingComponent;
class UFootstepAssetBank;

UCLASS()
class PROJECTPA_API AGameCharacterBase : public ACharacter, public IAbilitySystemInterface, public IPAActor, public IFootstepAssetProvider
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipmentComponent> EquipmentComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCharacterAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UFootstepAssetBank> HumanAnimationAudioBank;
	
	UPROPERTY(EditAnywhere, Category = Swimming)
	float ActorSwimLocationOffsetZ;

	UPROPERTY()
	TObjectPtr<UItemBase> EquippedItem;

	UPROPERTY(ReplicatedUsing = OnRep_UID)
	int32 ActorUID;

	UPROPERTY(ReplicatedUsing = OnRep_DeadStart)
	bool bCharacterDeadStart;
	UPROPERTY(ReplicatedUsing = OnRep_DeadEnd)
	bool bCharacterDeadEnd;
	UPROPERTY(visibleAnywhere)
	bool bIsSwimming;
	
public:
	AGameCharacterBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PossessedBy(AController* NewController) override;

	//~Begin IPAActor Interface
	virtual void RequestUID() override;
	virtual EUIDType GetContainingType() const override;
	virtual int32 GetUID() const override { return ActorUID; }
	virtual void SetUID(int32 NewUID) override { ActorUID = NewUID; }
	UFUNCTION()
	virtual void OnRep_UID() override;
	//~End IPAActor Interface

	//~Begin IAbilitySystemInterface Functions
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	template<class T>
	T* GetAbilitySystemComponent() const { return Cast<T>(GetAbilitySystemComponent()); }
	//~End IAbilitySystemInterface Functions
	
	//~Begin IAnimationAudioBank Functions
	virtual UFootstepAssetBank* GetFootstepAssetBank() const override { return HumanAnimationAudioBank; }
	virtual bool CanPlayFootstepSound() const override;
	//~End IAnimationAudioBank Functions 

	FTransform GetRootBoneTransform() const;
	FORCEINLINE UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }

	virtual EGetItemError AddItem(FItemEncodedInfo& ItemToAdd) { return EGetItemError::GeneralError; }
	virtual FItemEncodedInfo* GetItem(const FItemEncodedInfo& InItemEncodedInfo) { return nullptr; }
	virtual void RemoveItem(const FItemEncodedInfo& ItemToRemove) { }
	virtual void ForceAddItem(FItemEncodedInfo& ItemToAdd) { }
	virtual void EquipItem(const FItemEncodedInfo& ItemToEquip, const int32 ItemOwnerUID) { }
	virtual void UnEquipItem(FItemEncodedInfo& ItemToEquip) { }
	virtual void WearItem(const FItemEncodedInfo& ItemToWear, const int32 ItemOwnerUID) { }
	virtual void UnWearItem(FItemEncodedInfo& ItemToWear) { }
	
	/**
	 * 지정된 타입의 아이템 중 Functor의 비교 규칙에 따라 가장 좋은 아이템을 찾습니다.
	 * @param Comparator 아이템 우선순위를 결정하는 Functor 객체입니다.
	 * @return 찾은 아이템 정보에 대한 포인터, 없으면 nullptr을 반환합니다.
	 */
	virtual const FItemEncodedInfo* FindBestItem(const EItemType ItemType, EItemComparisonLogic Comparator) const { return nullptr; }
    
	
	virtual void StopAllActions();
	virtual void PauseAllActions() { }

	virtual void StartDying();
	virtual void DyingEnd();

	virtual void StartSwimming(TScriptInterface<ISwimmableVolume> SwimmableVolume);
	virtual void StopSwimming(TScriptInterface<ISwimmableVolume> SwimmableVolume);
	bool IsSwimming() const { return bIsSwimming; }
	virtual bool IsSprinting() const { return false; }
	
protected:
	virtual void BeginPlay() override;
	virtual void PostBeginPlay();

	void SetSwimming(bool bSwimming);
	
	virtual void OnAbilitySystemReady() { }
	
	/**
	 * 캐릭터 사망 시, 이 캐릭터가 가진 아이템을 어디에 저장할지 함수입니다.
	 * @param ItemInfos 장비에서 해제된 아이템 정보입니다.
	 */
	virtual void AddItemsToLootOnDeath(TArray<FItemEncodedInfo>& ItemInfos) { }
	
	// Replication Functions
	UFUNCTION()
	virtual void OnRep_DeadStart() {}
	UFUNCTION()
	virtual void OnRep_DeadEnd() {}
	
private:
	/** 사망 시, 장착 중인 모든 장비를 해제하여 각 캐릭터의 루팅 목록으로 옮깁니다. */
	void UnequipAndTransferAllItemsOnDeath();
};
