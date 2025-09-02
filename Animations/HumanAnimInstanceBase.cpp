#include "HumanAnimInstanceBase.h"
#include "HumanAnimExtraData.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PoseSearch/PoseSearchTrajectoryLibrary.h"
#include "ProjectPA/PALog.h"

#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ProjectPA/Generals/PAUtils.h"

void UHumanAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedCharacter = Cast<AGameCharacterBase>(GetOwningActor());
	if (IsValid(CachedCharacter))
	{
		CachedMovementComponent = CachedCharacter->GetCharacterMovement();
		CachedCharacter->LandedDelegate.AddDynamic(this, &UHumanAnimInstanceBase::OnCharacterLanded);
	}
	const TObjectPtr<UDataAsset>& loadedData = UPAUtils::LoadDataAsset(TEXT("/Game/GameCharacters/ExtraData/DA_PlayerAnimExtraData.DA_PlayerAnimExtraData"));
	if (loadedData)
	{
		ExtraData = Cast<UHumanAnimExtraData>( loadedData );
	}
}

void UHumanAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateTrajectory(DeltaSeconds);

	if (IsValid(CachedMovementComponent))
	{
		Velocity = CachedMovementComponent->Velocity;
		Accleration = CachedMovementComponent->GetCurrentAcceleration().Size();
		GroundSpeed = Velocity.Size2D();
		bIsFalling = CachedMovementComponent->IsFalling();
		bPreviousMoved = bShouldMove;
		bShouldMove = (CachedMovementComponent->GetCurrentAcceleration().IsZero() == false) ||
						GroundSpeed > 3.0f;
		if (CachedMovementComponent->bOrientRotationToMovement == false && GetOwningActor())
		{
			DirectionYaw = UKismetAnimationLibrary::CalculateDirection(Velocity, GetOwningActor()->GetActorRotation());
		}
		bIsCrouching = CachedMovementComponent->IsCrouching();
		bOrientRotationToMovement = CachedMovementComponent->bOrientRotationToMovement;
	}
	if (IsValid(CachedCharacter))
	{
		bIsSwimming = CachedCharacter->IsSwimming();
		CharacterTransform = CachedCharacter->GetActorTransform();

		const float CharacterYaw = CachedCharacter->GetTransform().GetRotation().Rotator().Yaw;
		const float RootYaw = CachedCharacter->GetRootBoneTransform().GetRotation().Rotator().Yaw;
	}

	// Set Future Velocity
	UPoseSearchTrajectoryLibrary::GetTransformTrajectoryVelocity(Trajectory, 0.4f, 0.5f, FutureTrajectoryVelocity);
}

void UHumanAnimInstanceBase::UpdateTrajectory(float DeltaSeconds)
{
	// Do Trajectory First
	FPoseSearchTrajectoryData PoseData;
	UPoseSearchTrajectoryLibrary::PoseSearchGenerateTransformTrajectory(
	this,
	IsMoving() ? TrajectoryGenerationData_Moving : TrajectoryGenerationData_Idle,
	DeltaSeconds,
	Trajectory,
	DesiredControllerYaw,
	Trajectory,
	-1.0f,
	30,
	0.1f,
	15
	);

	// Do Collision Trajectory (No Calculate At Simulated Proxy Due To Optimization)
	if (IsValid(CachedCharacter) == false || CachedCharacter->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}
	
	TArray<AActor*> IgnoreActors{ GetOwningActor() };
	UPoseSearchTrajectoryLibrary::HandleTransformTrajectoryWorldCollisions(
	GetOwningActor(),
	this,
	Trajectory,
	true,
	0.01f,
	Trajectory,
	TrajectoryCollision,
	TraceTypeQuery1,
	false,
	IgnoreActors,
	EDrawDebugTrace::None,
	true,
	150.0f
	);
	
	// UPoseSearchTrajectoryLibrary::HandleTrajectoryWorldCollisions(
	// 	GetOwningActor(),
	// 	this,
	// 	Trajectory,
	// 	true,
	// 	0.01f,
	// 	Trajectory,
	// 	TrajectoryCollision,
	// 	ETraceTypeQuery::TraceTypeQuery1,
	// 	false,
	// 	IgnoreActors,
	// 	EDrawDebugTrace::None,
	// 	true,
	// 	150.0f
	// );
}

void UHumanAnimInstanceBase::PlayFootstepSound( const EPhysicalSurface surfaceType, const FVector& location )
{
	if ( ExtraData == nullptr )
	{
		return;
	}
	if ( ExtraData->FootStepSoundMap.Contains( surfaceType ) == false )
	{
		return;
	}
	if ( ExtraData->FootStepSoundMap[surfaceType] == nullptr )
	{
		return;
	}
	
	UGameplayStatics::PlaySoundAtLocation( GetWorld(), ExtraData->FootStepSoundMap[surfaceType], location );
}

bool UHumanAnimInstanceBase::IsStarting() const
{
	const bool bVelocityWillIncrease = FutureTrajectoryVelocity.Size2D() >= GroundSpeed + 100.0f;
	const bool bContainPivots = CurrentPSDTags.Contains(TEXT("Pivots"));

	return IsMoving() && bVelocityWillIncrease && bContainPivots == false;
}

bool UHumanAnimInstanceBase::IsPivoting() const
{
	return FMath::Abs(GetTrajectoryTurnAngle()) >= (bOrientRotationToMovement ? 45.0f : 30.0f);
}

bool UHumanAnimInstanceBase::IsMoving() const
{
	bool bWillStop = FutureTrajectoryVelocity.Equals(FVector::ZeroVector, 10.0f);
	return Accleration > 0.0f && !bWillStop;
}

bool UHumanAnimInstanceBase::ShouldTurnInPlace() const
{
	if (IsValid(CachedCharacter) && bShouldMove == false && bPreviousMoved)
	{
		const float CharacterYaw = CachedCharacter->GetTransform().GetRotation().Rotator().Yaw;
		const float RootYaw = CachedCharacter->GetRootBoneTransform().GetRotation().Rotator().Yaw;
		return FMath::Abs(CharacterYaw - RootYaw) >= 40.0f;
	}
	return false;
}

bool UHumanAnimInstanceBase::ShouldSpinTransition() const
{
	if (IsValid(CachedCharacter))
	{
		const float CompareAngle = CachedCharacter->IsSwimming() ? 15.0f : 60.0f;
		const float CharacterYaw = CachedCharacter->GetTransform().GetRotation().Rotator().Yaw;
		const float RootYaw = CachedCharacter->GetRootBoneTransform().GetRotation().Rotator().Yaw;
		const bool bDeltaYawOffset = FMath::Abs(CharacterYaw - RootYaw) >= CompareAngle;
		const bool bSpeed = GroundSpeed >= 150.0f;

		const bool bContainPivots = CurrentPSDTags.Contains(TEXT("Pivots"));
		
		return bDeltaYawOffset && bSpeed && bContainPivots == false;
	}
	return false;
}

bool UHumanAnimInstanceBase::JustParkoured() const
{
	return (IsSlotActive(TEXT("DefaultSlot")) == false) && GetCurveValue(TEXT("MovingTraversal")) > 0 && GetTrajectoryTurnAngle() <= 50;
}

float UHumanAnimInstanceBase::GetMotionMatchingBlendTime() const
{
	if (!bIsFalling)
	{
		return bJustLanded ? 0.2f : 0.5f;
	}
	return Velocity.Z > 100.0f ? 0.15f : 0.5f;
}

void UHumanAnimInstanceBase::OnCharacterLanded(const FHitResult& Hit)
{
	bJustLanded = true;
	bLandedHeavy = Velocity.Z >= HeavyLandSpeed;
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(LandTimer, this, &UHumanAnimInstanceBase::OnLandedEnd, LandClearTime);
	}
}

void UHumanAnimInstanceBase::OnLandedEnd()
{
	bJustLanded = false;
	bLandedHeavy = false;
}

float UHumanAnimInstanceBase::GetTrajectoryTurnAngle() const
{
	return FMath::Abs(Velocity.ToOrientationRotator().Yaw - FutureTrajectoryVelocity.ToOrientationRotator().Yaw);
}
