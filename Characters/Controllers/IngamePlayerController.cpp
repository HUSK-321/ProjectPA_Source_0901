#include "IngamePlayerController.h"
#include "ProjectPA/Camera/PAPlayerCameraManager.h"

AIngamePlayerController::AIngamePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraManagerClass = APAPlayerCameraManager::StaticClass();
}
void AIngamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	Super::AddCheats(true);
}

void AIngamePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

