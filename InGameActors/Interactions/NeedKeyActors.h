// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Interfaces/InteractableActor.h"
#include "ProjectPA/InGameActors/PAActorBase.h"
#include "NeedKeyActors.generated.h"

class UItemBase;

UCLASS()
class PROJECTPA_API ANeedKeyActors : public APAActorBase, public IInteractableActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemBase> KeyItemToNeed;
	UPROPERTY(EditAnywhere)
	int32 KeyCode;
	
public:
	ANeedKeyActors();
	virtual void InteractWith(APlayerCharacter* InteractingPlayer) override;

	UFUNCTION(BlueprintNativeEvent)
	void Open();
};
