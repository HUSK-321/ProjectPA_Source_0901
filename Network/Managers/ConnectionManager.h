// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/InGameActors/PAActorBase.h"
#include "ConnectionManager.generated.h"

class UConnectionManagerBase;

UENUM(BlueprintType)
enum class EConnectionType : uint8
{
	Item
};

USTRUCT(Blueprintable, BlueprintType)
struct FManagerPair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EConnectionType Type;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UConnectionManagerBase> ManagerClass;
};

UCLASS()
class PROJECTPA_API AConnectionManager : public APAActorBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TArray<FManagerPair> ManagersToSpawn;
	UPROPERTY(EditAnywhere)
	TMap<EConnectionType, UConnectionManagerBase*> ConnectionManagers; 
	
public:
	AConnectionManager();

	UFUNCTION(Reliable, Server)
	virtual void SendServerRPC(EConnectionType Type, const TArray<uint8>& SerializedData) ;
	virtual void SendMulticastRPC(EConnectionType Type, const TArray<uint8>& SerializedData);
	UFUNCTION(Reliable, Client)
	virtual void ClientGetRPC(EConnectionType Type, const TArray<uint8>& SerializedData);

protected:
	virtual void BeginPlay() override;
};
