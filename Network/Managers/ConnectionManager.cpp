// All copyrights for this code are owned by Aster.
#include "ConnectionManager.h"
#include "ConnectionManagerBase.h"

AConnectionManager::AConnectionManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AConnectionManager::BeginPlay()
{
	Super::BeginPlay();

	ConnectionManagers.Empty();
	for (const FManagerPair& ManagerPair : ManagersToSpawn)
	{
		if (ManagerPair.ManagerClass)
		{
			if (UConnectionManagerBase* NewManager = NewObject<UConnectionManagerBase>(this, ManagerPair.ManagerClass))
			{
				ConnectionManagers.Add({ManagerPair.Type, NewManager});
				NewManager->InitializeManager(this);
			}
		}
	}
	ManagersToSpawn.Empty();
}

void AConnectionManager::SendServerRPC_Implementation(EConnectionType Type, const TArray<uint8>& SerializedData)
{
	if (ConnectionManagers.Contains(Type))
	{
		ConnectionManagers[Type]->ServerGetRPC(SerializedData);
	}
}

void AConnectionManager::SendMulticastRPC(EConnectionType Type, const TArray<uint8>& SerializedData)
{
	if (HasAuthority() == false)
	{
		return;
	}
}

void AConnectionManager::ClientGetRPC_Implementation(EConnectionType Type, const TArray<uint8>& SerializedData)
{
	// TODO : 현재 오너가 SerializedDat를 받는 놈인지 확인
	if (ConnectionManagers.Contains(Type))
	{
		ConnectionManagers[Type]->ClientGetRPC(SerializedData);
	}
}
