// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PACharacterMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTPA_API UPACharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPACharacterMovementComponent();

protected:
	virtual void BeginPlay() override;
};
