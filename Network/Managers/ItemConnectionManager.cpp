// All copyrights for this code are owned by Aster.


#include "ItemConnectionManager.h"
#include "ConnectionManager.h"
#include "ProjectPA/Interfaces/ItemContainer.h"
#include "ProjectPA/Network/ConnectionProtocolTypes.h"
#include "ProjectPA/Network/UIDContainer.h"

UItemConnectionManager::UItemConnectionManager()
{
}

void UItemConnectionManager::ServerGetRPC(const TArray<uint8>& SerializedData)
{
	if (IsValid(ConnectionManagerActor) == false || ConnectionManagerActor->HasAuthority() == false)
	{
		return;
	}

	if (SerializedData.Num() >= 2)
	{
		EItemProtocolType ProtocolType = static_cast<EItemProtocolType>(SerializedData[1]);
		switch (ProtocolType)
		{
		case EItemProtocolType::PlayerGetItem :
			HandleItemMove(SerializedData);
			break;
		case EItemProtocolType::ItemUse :
			HandleItemUse(SerializedData);
			break;
		}
	}

	Super::ServerGetRPC(SerializedData);
}

void UItemConnectionManager::SendMulticastRPC(const TArray<uint8>& SerializedData)
{
	Super::SendMulticastRPC(SerializedData);
}

void UItemConnectionManager::ClientGetRPC(const TArray<uint8>& SerializedData)
{
	Super::ClientGetRPC(SerializedData);
}

void UItemConnectionManager::HandleItemMove(const TArray<uint8>& SerializedData)
{
	FPlayerGetItemProtocol ItemMoveProtocol;
	FMemoryReader Reader(SerializedData);
	ItemMoveProtocol.Serialize(Reader);

	UUIDContainer* UIDContainer = UUIDContainer::GetInstance(this);
	if (UIDContainer == nullptr)
	{
		return;
	}

	TScriptInterface<IPAActor> OriginItemOwner = UIDContainer->GetObjectByUID(ItemMoveProtocol.ContainerType, ItemMoveProtocol.ContainerUID);
	TScriptInterface<IPAActor> NewItemOwner =  UIDContainer->GetObjectByUID(EUIDType::Character, ItemMoveProtocol.PlayerUID);
	if (OriginItemOwner.GetObject() == nullptr || NewItemOwner.GetObject() == nullptr || OriginItemOwner.GetInterface() == nullptr || NewItemOwner.GetInterface() == nullptr)
	{
		return;
	}

	// 원래 주인을 찾아서 아이템이 있는지 확인 후 제거
	IItemContainer* OriginContainer = Cast<IItemContainer>(OriginItemOwner.GetObject());
	if (OriginContainer == nullptr)
	{
		return;
	}

	// TODO : ItemEncodedInfo 가 잘 직렬화 되는가
	bool bHaveItem = OriginContainer->HasItem(ItemMoveProtocol.ItemEncodedInfo);
	if (bHaveItem == false)
	{
		return;
	}

	OriginContainer->RemoveItem(ItemMoveProtocol.ItemEncodedInfo);

	// 이후 플레이어가 아이템을 얻을 수 있게
	// TODO : 기존에 만들어진 아이템을 먹게 하고 싶지만 위 RemoveItem 후 리플리케이트 되면 클라이언트에서는 아이템이 사라지게 된다
}

void UItemConnectionManager::HandleItemUse(const TArray<uint8>& SerializedData)
{
}
