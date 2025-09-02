#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/TrajectoryTypes.h"
#include "PoseSearch/PoseSearchTrajectoryLibrary.h"
#include "PoseSearch/PoseSearchTrajectoryTypes.h"
#include "HumanAnimInstanceBase.generated.h"

class AGameCharacterBase;
class UCharacterMovementComponent;
class UHumanAnimExtraData;

UCLASS()
class PROJECTPA_API UHumanAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CachedMovementComponent;
	UPROPERTY()
	TObjectPtr<AGameCharacterBase> CachedCharacter;

	// Trajectory
	UPROPERTY(EditAnywhere, Category = Trajectory)
	FPoseSearchTrajectoryData TrajectoryGenerationData_Idle;
	UPROPERTY(EditAnywhere, Category = Trajectory)
	FPoseSearchTrajectoryData TrajectoryGenerationData_Moving;
	UPROPERTY(BlueprintReadOnly, Category = Trajectory, meta = (AllowPrivateAccess = true))
	FTransformTrajectory Trajectory;
	//FPoseSearchQueryTrajectory Trajectory;
	FPoseSearchTrajectory_WorldCollisionResults TrajectoryCollision;
	float DesiredControllerYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<FName> CurrentPSDTags;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	int32 AnimationLOD;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	FTransform CharacterTransform;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	FVector Velocity;
	FVector FutureTrajectoryVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	float Accleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	float DirectionYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	bool bPreviousMoved;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	bool bShouldMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	bool bIsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	bool bIsSwimming;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CahracterStat, meta=(AllowPrivateAccess=true))
	bool bOrientRotationToMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float HeavyLandSpeed = 700.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float LandClearTime = 0.3f;
	FTimerHandle LandTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bJustLanded;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bLandedHeavy;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<UHumanAnimExtraData> ExtraData;

public:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void UpdateTrajectory(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void PlayFootstepSound( const EPhysicalSurface surfaceType, const FVector& location );
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool IsStarting() const;
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool IsPivoting() const;
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool IsMoving() const;
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool ShouldTurnInPlace() const;
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool ShouldSpinTransition() const;
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool JustParkoured() const;
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	float GetTimeToLand() const { return bIsFalling ? TrajectoryCollision.TimeToLand : 0.0f; }
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	float GetMotionMatchingBlendTime() const;
	
private:
	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);
	void OnLandedEnd();

	float GetTrajectoryTurnAngle() const;
};
