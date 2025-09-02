// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Generals/PATypes.h"
#include "UObject/Object.h"
#include "ConnectionProtocolTypes.generated.h"


USTRUCT()
struct FPlayerGetItemProtocol
{
	GENERATED_BODY()

	uint8 ConnectionType;	// Not-Used
	uint8 ProtocolType;		// Not-Used
	uint8 ContainerType;	// EUIDType
	int32 ContainerUID;
	int32 PlayerUID;
	FItemEncodedInfo ItemEncodedInfo;

	void Serialize(FArchive& Ar)
	{
		Ar << ConnectionType;
		Ar << ProtocolType;
		Ar << ContainerUID;
		Ar << PlayerUID;
		ItemEncodedInfo.Serialize(Ar);
	}
};

