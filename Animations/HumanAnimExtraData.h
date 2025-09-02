// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HumanAnimExtraData.generated.h"

/**
 * AnimBP에 사용되는 데이터들을 정리하는 데이터 에셋입니다.
 */
UCLASS()
class PROJECTPA_API UHumanAnimExtraData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Footstep)
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> FootStepSoundMap;
};
