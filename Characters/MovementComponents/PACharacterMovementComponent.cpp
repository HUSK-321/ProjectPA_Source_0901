// All copyrights for this code are owned by Aster.

#include "PACharacterMovementComponent.h"

UPACharacterMovementComponent::UPACharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bCanWalkOffLedges = false;
	bCanWalkOffLedgesWhenCrouching = false;
}

void UPACharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

