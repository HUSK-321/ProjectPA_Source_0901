// All copyrights for this code are owned by Aster.

#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "PlayerCameraComponent.generated.h"

#define CAMERA_DEFAULT_FOV			(80.0f)
#define CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define CAMERA_DEFAULT_PITCH_MAX	(89.0f)

class UCanvas;
class UCameraMode;
class UCameraModeStack;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FMinimalViewInfo;
template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UCameraMode>, FCameraModeDelegate);

UCLASS(Blueprintable, ClassGroup=(Camera), meta=(BlueprintSpawnableComponent))
class PROJECTPA_API UPlayerCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

protected:
	// Stack used to blend the camera modes.
	UPROPERTY()
	TObjectPtr<UCameraModeStack> CameraModeStack;
	// Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	float FieldOfViewOffset;

public:
	UPlayerCameraComponent();

	// Returns the camera component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Camera")
	static UPlayerCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UPlayerCameraComponent>() : nullptr); }

	// Returns the target actor that the camera is looking at.
	virtual AActor* GetTargetActor() const { return GetOwner(); }
	// Delegate used to query for the best camera mode.
	FCameraModeDelegate DetermineCameraModeDelegate;
	// Add an offset to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }
	virtual void DrawDebug(UCanvas* Canvas) const;
	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void UpdateCameraModes();

};
