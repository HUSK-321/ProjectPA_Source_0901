// All copyrights for this code are owned by Aster.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "PAPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS(notplaceable)
class PROJECTPA_API APAPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	APAPlayerCameraManager();

protected:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;
	
};
