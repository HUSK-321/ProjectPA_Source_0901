// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FootstepAssetProvider.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFootstepAssetProvider : public UInterface
{
	GENERATED_BODY()
};

class UFootstepAssetBank;

class PROJECTPA_API IFootstepAssetProvider
{
	GENERATED_BODY()

public:
	virtual const UFootstepAssetBank* GetFootstepAssetBank() const { return nullptr; }
	virtual bool CanPlayFootstepSound() const { return false; }
};
