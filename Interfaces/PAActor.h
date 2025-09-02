// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PAActor.generated.h"

UINTERFACE()
class UPAActor : public UInterface
{
	GENERATED_BODY()
};

enum class EUIDType : uint8;

class PROJECTPA_API IPAActor
{
	GENERATED_BODY()

public:
	// Function should be called on BeginPlay()
	virtual void RequestUID() = 0;
	virtual EUIDType GetContainingType() const = 0;
	virtual int32 GetUID() const { return INDEX_NONE; }
	virtual void SetUID(int32 NewUID) { }

	UFUNCTION()
	virtual void OnRep_UID() { }
};
