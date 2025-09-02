// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ConnectionManagerBase.h"
#include "ItemConnectionManager.generated.h"

UENUM()
enum class EItemProtocolType : uint8
{
	// 아이템이 A -> B 로 이동하게 되는 경우
	PlayerGetItem,
	// 아이템을 사용하는 경우
	ItemUse,
};

UCLASS()
class PROJECTPA_API UItemConnectionManager : public UConnectionManagerBase
{
	GENERATED_BODY()

public:
	UItemConnectionManager();
	
	virtual void ServerGetRPC(const TArray<uint8>& SerializedData) override;
	virtual void SendMulticastRPC(const TArray<uint8>& SerializedData) override;
	virtual void ClientGetRPC(const TArray<uint8>& SerializedData) override;

private:
	void HandleItemMove(const TArray<uint8>& SerializedData);
	void HandleItemUse(const TArray<uint8>& SerializedData);
};
