#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"

class IItemContainer;
class APlayerCharacter;
class IInteractableActor;
class AItemActorBase;
class UInteractionComponent;

USTRUCT()
struct FItemContainerInfo
{
	GENERATED_BODY()

	FItemContainerInfo(){}

	FItemContainerInfo(const TScriptInterface<IItemContainer>& InContainer, FDelegateHandle& InDelegate)
		:
		ItemContainer(InContainer), OnItemChangedDelegateHandle(InDelegate)
	{}

	bool operator==(const FItemContainerInfo& Other) const
	{
		return (ItemContainer == Other.ItemContainer);
	}

	UPROPERTY()
	TScriptInterface<IItemContainer> ItemContainer;
	FDelegateHandle OnItemChangedDelegateHandle;
};

DECLARE_EVENT(UInteractionComponent, FOnNearbyItemListChanged)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTPA_API UInteractionComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	FOnNearbyItemListChanged OnNearbyItemListChanged;
	
private:
	/** 플레이어 주변 확인 가능한 아이템들 */
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess=true))
	TArray<FItemContainerInfo> NearbyItemList;
	/** 플레이어 주변 상호작용 가능한 액터들 */
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess=true))
	TArray<TScriptInterface<IInteractableActor>> NearbyInteractActors;
	UPROPERTY()
	TObjectPtr<APlayerCharacter> CachedOwnerPlayer;
	
public:
	UInteractionComponent();

	void TryInteract(APlayerCharacter* InteractionCharacter);

	TArray<TScriptInterface<IItemContainer>> GetNearbyItemList() const;

protected:
	virtual void BeginPlay() override;

private:
	void BroadcastNearbyItemListChanged();
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
