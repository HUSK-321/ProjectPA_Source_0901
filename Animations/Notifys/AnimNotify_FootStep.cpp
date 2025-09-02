// All copyrights for this code are owned by Aster.


#include "AnimNotify_FootStep.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectPA/Data/Audio/FootstepAssetBank.h"
#include "ProjectPA/Interfaces/FootstepAssetProvider.h"

UAnimNotify_FootStep::UAnimNotify_FootStep()
	: TraceDistance(50.0f)
	, VolumeMultiplier(1.0f)
	, HearingStimulusRange(1000.0f)
	, SoundLoudness(1.0f)
	, bBothFootStep(false)
	, bLeft(true)
	, bMakeNoise(false)
	, bPlayFootStepVFX(false)
	#if WITH_EDITORONLY_DATA
	, bDrawDebugTrace(false)
#endif
{
}

void UAnimNotify_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IsValid(MeshComp) == false)
	{
		return;
	}
	AActor* OwnerActor = MeshComp->GetOwner();
	if (IsValid(OwnerActor) == false)
	{
		return;
	}
	const UWorld* World = OwnerActor->GetWorld();
	if (IsValid(World) == false)
	{
		return;
	}

	const FName CurrentFootBoneName = bLeft ? TEXT("l_foot") : TEXT("r_foot");
	const FVector FootLocation = MeshComp->GetSocketLocation(CurrentFootBoneName);

	FHitResult HitResult;
	EPhysicalSurface HitSurfaceType = SurfaceType_Default;
	const bool bGroundHit = PerformGroundTrace(World, HitResult, FootLocation, TraceDistance, HitSurfaceType, OwnerActor);
	
	if (bGroundHit)
	{
		PlayFootstepEffects(OwnerActor, FootLocation, HitSurfaceType);
	}
	
	if (bBothFootStep)
	{
		const FName OtherSideBoneName = !bLeft ? TEXT("l_foot") : TEXT("r_foot");
		const FVector OtherSideFootLocation = MeshComp->GetSocketLocation(OtherSideBoneName);
		const bool bOtherSideGroundHit = PerformGroundTrace(World, HitResult, OtherSideFootLocation, TraceDistance, HitSurfaceType, OwnerActor);

		if (bOtherSideGroundHit)
		{
			PlayFootstepEffects(OwnerActor, FootLocation, HitSurfaceType);
		}
	}
}

bool UAnimNotify_FootStep::PerformGroundTrace(const UWorld* World, FHitResult& HitResult, const FVector& StartLoc, const float TraceRange,
                                              EPhysicalSurface& OutSurfaceType, const AActor* IgnoredActor) const
{
	if (IsValid(World) == false)
	{
		return false;
	}

	FVector EndLoc = StartLoc - FVector(0, 0, TraceRange);

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(IgnoredActor);

#if WITH_EDITORONLY_DATA
	if (bDrawDebugTrace)
	{
		bool bDebugHit = World->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, Params);
		FColor DrawColor = bDebugHit ? FColor::Red : FColor::Green;
		DrawDebugLine(World, StartLoc, EndLoc, DrawColor, false, 0.5f, 0, 1.0f);
		if (bDebugHit)
		{
			DrawDebugPoint(World, HitResult.ImpactPoint, 10.0f, DrawColor, false, 0.5f);
		}
	}
#endif

	const bool bHit = World->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, Params);

	if (bHit)
	{
		OutSurfaceType = UGameplayStatics::GetSurfaceType(HitResult);
		return true;
	}
	return false;
}

void UAnimNotify_FootStep::PlayFootstepEffects(AActor* InOwnerActor, const FVector& InLocation, EPhysicalSurface PhysicalSurface) const
{
	const IFootstepAssetProvider* AssetProvider = Cast<IFootstepAssetProvider>(InOwnerActor);
	if (AssetProvider == nullptr ||  AssetProvider->CanPlayFootstepSound() == false)
	{
		return;
	}

	const UFootstepAssetBank* FootstepAssetBank = AssetProvider->GetFootstepAssetBank();
	if (IsValid(FootstepAssetBank))
	{
		USoundBase* SoundToPlay = FootstepAssetBank->GetSound(AnimEventTag);
		if (IsValid(SoundToPlay))
		{
			UGameplayStatics::PlaySoundAtLocation(InOwnerActor, SoundToPlay, InLocation);
		}

		if (bPlayFootStepVFX)
		{
			UNiagaraSystem* NiagaraFXToSpawn = FootstepAssetBank->GetNiagaraFX(AnimEventTag, PhysicalSurface);
			if (IsValid(NiagaraFXToSpawn))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(InOwnerActor->GetWorld(), NiagaraFXToSpawn, InLocation, InOwnerActor->GetActorRotation());
			}	
		}
	}
	
	if (bMakeNoise)
	{
		APawn* InstigatorPawn = Cast<APawn>(InOwnerActor);
		if (IsValid(InstigatorPawn))
		{
			InOwnerActor->MakeNoise(SoundLoudness, InstigatorPawn, InLocation, HearingStimulusRange, FName("Footstep"));
		}	
	}
}
