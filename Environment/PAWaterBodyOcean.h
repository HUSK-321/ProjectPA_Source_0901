// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "WaterBodyOceanActor.h"
#include "ProjectPA/Interfaces/SwimmableVolume.h"
#include "PAWaterBodyOcean.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API APAWaterBodyOcean : public AWaterBodyOcean, public ISwimmableVolume
{
	GENERATED_BODY()
	
public:
	APAWaterBodyOcean();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	// ~Begin ISwimmableVolume interfaces
	virtual float GetSwimmableLocationZ() const override { return GetActorLocation().Z; }
	// ~End ISwimmableVolume interfaces
};
