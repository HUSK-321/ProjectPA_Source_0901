// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SwimmableVolume.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USwimmableVolume : public UInterface
{
	GENERATED_BODY()
};


class PROJECTPA_API ISwimmableVolume
{
	GENERATED_BODY()

public:
	virtual float GetSwimmableLocationZ() const { return 0.0f; }
};
