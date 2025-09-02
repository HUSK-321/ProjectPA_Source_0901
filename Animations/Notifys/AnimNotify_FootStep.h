// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FootStep.generated.h"

class UNiagaraSystem;

UCLASS()
class PROJECTPA_API UAnimNotify_FootStep : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_FootStep();

	// ~Begin UAnimNotify Functions
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	// ~End UAnimNotify Functions
	
protected:
	/** 해당하는 Anim 의 Event Gameplay Tag */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	FGameplayTag AnimEventTag;
	/** 발자국 Trace 를 할 때 지면으로 트레이스할 거리 (위에서 아래로) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	float TraceDistance;
	/** 발자국 소리 Multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	float VolumeMultiplier;
	/** AI Hearing Sense에 발생시킬 소리의 반경 (Hearing Range) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep|AI Perception")
	float HearingStimulusRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep|AI Perception")
	float SoundLoudness;
	/** 이 노티파이가 양발에 해당하는지 지정합니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	bool bBothFootStep;
	/** 이 노티파이가 왼발(true)에 대한 것인지 오른발(false)에 대한 것인지 지정합니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep", meta = (EditCondition = "!bBothFootStep"))
	bool bLeft;
	/** true 시 AI 들이 해당 소리를 듣게 됩니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	bool bMakeNoise;
	/** 발자국  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	bool bPlayFootStepVFX;
#if WITH_EDITORONLY_DATA
	/** 디버그 라인 트레이스 드로우 활성화 여부 (에디터 전용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep|Debug")
	bool bDrawDebugTrace;
#endif

private:
	bool PerformGroundTrace(const UWorld* World, FHitResult& HitResult, const FVector& StartLoc, float TraceRange, EPhysicalSurface& OutSurfaceType, const
	                        AActor* IgnoredActor) const;
	void PlayFootstepEffects(AActor* InOwnerActor, const FVector& InLocation, EPhysicalSurface PhysicalSurface) const;
};
