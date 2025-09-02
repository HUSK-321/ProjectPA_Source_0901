// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ParkourMontageBlendOut.generated.h"

UENUM()
enum class EParkourBlendOutCondition : uint8
{
	ForceBlendOut,
	WithMovementInput,
	IfFalling
};

/**
 * 
 */
UCLASS()
class PROJECTPA_API UAnimNotifyState_ParkourMontageBlendOut : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	EParkourBlendOutCondition ParkourBlendOutCondition = EParkourBlendOutCondition::ForceBlendOut;
	UPROPERTY(EditAnywhere)
	float BlendOutTime = 0.2f;
	UPROPERTY(EditAnywhere)
	FName BlendProfileName = TEXT("FastFeet_InstantRoot");
	
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;

private:
	bool IsAccelerationIsZero(const ACharacter* Character) const;
	bool IsCharacterFalling(const ACharacter* Character) const;
};
