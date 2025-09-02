// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableActor.generated.h"

class APlayerCharacter;
// This class does not need to be modified.
UINTERFACE()
class UInteractableActor : public UInterface
{
	GENERATED_BODY()
};

class PROJECTPA_API IInteractableActor
{
	GENERATED_BODY()

public:
	virtual void InteractWith(APlayerCharacter* InteractingPlayer) {}
	virtual void OnInteractStart(APlayerCharacter* InteractingPlayer) {}
	virtual void OnInteractStop(APlayerCharacter* InteractingPlayer) {}
};
