#include "InteractionComponent.h"
#include "ProjectPA/Characters/PlayerCharacter.h"
#include "ProjectPA/Interfaces/InteractableActor.h"
#include "ProjectPA/Interfaces/ItemContainer.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SphereRadius = 150.f;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedOwnerPlayer = Cast<APlayerCharacter>(GetOwner());

	OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnSphereOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnSphereOverlapEnd);
}

void UInteractionComponent::TryInteract(APlayerCharacter* InteractionCharacter)
{
	for (auto& InteractionActor : NearbyInteractActors)
	{
		if (InteractionActor)
		{
			InteractionActor->InteractWith(InteractionCharacter);
		}
	}
}

TArray<TScriptInterface<IItemContainer>> UInteractionComponent::GetNearbyItemList() const
{
	TArray<TScriptInterface<IItemContainer>> ReturnArray;

	for (auto& ItemContainer : NearbyItemList)
	{
		ReturnArray.Emplace(ItemContainer.ItemContainer);
	}

	return ReturnArray;
}

void UInteractionComponent::BroadcastNearbyItemListChanged()
{
	OnNearbyItemListChanged.Broadcast();
}

void UInteractionComponent::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const TScriptInterface<IItemContainer> ItemContainerActor = OtherActor;
	if (ItemContainerActor.GetObject() != nullptr && ItemContainerActor.GetInterface() != nullptr)
	{
		FSimpleDelegate NewDelegate = FSimpleDelegate::CreateUObject(this, &UInteractionComponent::BroadcastNearbyItemListChanged);
		FDelegateHandle DelegateHandle = ItemContainerActor->AddItemChangedDelegate(NewDelegate);

		const FItemContainerInfo NewContainer{ ItemContainerActor, DelegateHandle };
		NearbyItemList.AddUnique(NewContainer);
		ItemContainerActor->SetOverlayEffect(true);
		BroadcastNearbyItemListChanged();
	}

	const TScriptInterface<IInteractableActor> InteractableActor = OtherActor;
	if (InteractableActor.GetObject() != nullptr && InteractableActor.GetInterface() != nullptr)
	{
		NearbyInteractActors.AddUnique(InteractableActor);
		InteractableActor->OnInteractStart(CachedOwnerPlayer);
	}
}

void UInteractionComponent::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const TScriptInterface<IItemContainer> ItemContainerActor = OtherActor;
	if (ItemContainerActor.GetObject() != nullptr && ItemContainerActor.GetInterface() != nullptr)
	{
		for (int32 Index = 0; Index < NearbyItemList.Num(); ++Index)
		{
			if (NearbyItemList[Index].ItemContainer == ItemContainerActor)
			{
				ItemContainerActor->RemoveItemChangedDelegate(NearbyItemList[Index].OnItemChangedDelegateHandle);
				NearbyItemList.RemoveAt(Index);
				break;
			}
		}
		
		ItemContainerActor->SetOverlayEffect(false);
		BroadcastNearbyItemListChanged();
	}

	const TScriptInterface<IInteractableActor> InteractableActor = OtherActor;
	if (InteractableActor.GetObject() != nullptr && InteractableActor.GetInterface() != nullptr)
	{
		InteractableActor->OnInteractStop(CachedOwnerPlayer);
		NearbyInteractActors.Remove(InteractableActor);
	}
}