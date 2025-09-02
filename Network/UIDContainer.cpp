// All copyrights for this code are owned by Aster.
#include "UIDContainer.h"
#include "ProjectPA/PALog.h"

TObjectPtr<UUIDContainer> UUIDContainer::Instance = nullptr;

UUIDContainer* UUIDContainer::GetInstance(UObject* Context)
{
	if (Context == nullptr){
		return nullptr;
	}
      
	if (Instance == nullptr )
	{
		Instance = NewObject<UUIDContainer>(GetTransientPackage(), UUIDContainer::StaticClass());
		Instance->AddToRoot();
	}
	return Instance.Get();
}

TScriptInterface<IPAActor> UUIDContainer::GetObjectByUID(EUIDType UIDType, int32 UID)
{
	TMap<int32, TScriptInterface<IPAActor>>& UIDContainer = GetContainer(UIDType);
	if (UIDContainer.Contains(UID))
	{
		return UIDContainer[UID];
	}

	return nullptr;
}

TScriptInterface<IPAActor> UUIDContainer::GetObjectByUID(uint8 UIDType, int32 UID)
{
	return GetObjectByUID(static_cast<EUIDType>(UIDType), UID);
}

void UUIDContainer::RegisterObject(EUIDType UIDType, IPAActor* InActor)
{
	TMap<int32, TScriptInterface<IPAActor>>& SelectedContainer = GetContainer(UIDType);
	const int32 NewUID = GetAvailableUID();
	InActor->SetUID(NewUID);
	
	UObject* InObject = Cast<UObject>(InActor);
	SelectedContainer.Add({NewUID, InObject});
	ActorContainer.Add({NewUID, InObject});
}

void UUIDContainer::OnClientReplicatedUID(EUIDType UIDType, AActor* InActor)
{
	if (IsValid(InActor) == false || InActor->HasAuthority() == false)
	{
		return;
	}

	TScriptInterface<IPAActor> PAActor(InActor);
	if (PAActor.GetObject() && PAActor.GetInterface())
	{
		GetContainer(UIDType).Add({PAActor->GetUID(), PAActor});
		ActorContainer.Add({PAActor->GetUID(), PAActor});
	}
}

TScriptInterface<IPAActor> UUIDContainer::GetObjectByUID(int32 UID)
{
	return ActorContainer.Contains(UID) ? ActorContainer[UID] : nullptr;
}

int32 UUIDContainer::GetAvailableUID()
{
	int32 ReturnUID = AvailableUID;
	AvailableUID += 1;
	return ReturnUID;
}

TMap<int32, TScriptInterface<IPAActor>>& UUIDContainer::GetContainer(EUIDType UIDType)
{
	switch (UIDType)
	{
	case EUIDType::Character :
		return CharacterContainer;
	case EUIDType::ItemContainer :
		return ItemContainer;
	}

	PALog_E(TEXT("You passed wrong Type : %d"), UIDType);
	return ItemContainer;
}
