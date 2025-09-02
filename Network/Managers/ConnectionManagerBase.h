// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ConnectionManagerBase.generated.h"

class AConnectionManager;

UCLASS()
class PROJECTPA_API UConnectionManagerBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<AConnectionManager> ConnectionManagerActor;

public:
	UConnectionManagerBase();
	virtual void InitializeManager(AConnectionManager* InConnectionManagerActor);
	
	virtual void ServerGetRPC(const TArray<uint8>& SerializedData) {}
	virtual void SendMulticastRPC(const TArray<uint8>& SerializedData) {}
	virtual void ClientGetRPC(const TArray<uint8>& SerializedData) {}
	
};
