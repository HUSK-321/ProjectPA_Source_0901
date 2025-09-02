// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CameraMode.generated.h"

class UPlayerCameraComponent;

UENUM(BlueprintType)
enum class ECameraModeBlendFunction : uint8
{
	// Does a simple linear interpolation.
	Linear,
	// Immediately accelerates, but smoothly decelerates into the target.  Ease amount controlled by the exponent.
	EaseIn,
	// Smoothly accelerates, but does not decelerate into the target.  Ease amount controlled by the exponent.
	EaseOut,
	// Smoothly accelerates and decelerates.  Ease amount controlled by the exponent.
	EaseInOut,
	MAX		UMETA(Hidden)
};

/**
 * FCameraModeView
 *	View data produced by the camera mode that is used to blend camera modes.
 */
struct FCameraModeView
{
public:
	FCameraModeView();
	void Blend(const FCameraModeView& Other, float OtherWeight);

public:
	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

UCLASS(Blueprintable, BlueprintType)
class PROJECTPA_API UCameraMode : public UObject
{
	GENERATED_BODY()

protected:
	// A tag that can be queried by gameplay code that cares when a kind of camera mode is active
	// without having to ask about a specific mode (e.g., when aiming downsights to get more accuracy)
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	FGameplayTag CameraTypeTag;
	// View output produced by the camera mode.
	FCameraModeView View;
	// The horizontal field of view (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;
	// Minimum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;
	// Maximum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;
	// How long it takes to blend in this mode.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;
	// Function used for blending.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	ECameraModeBlendFunction BlendFunction;
	// Exponent used by blend functions to control the shape of the curve.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;
	// Linear blend alpha used to determine the blend weight.
	float BlendAlpha;
	// Blend weight calculated using the blend alpha and function.
	float BlendWeight;
	UPROPERTY(EditDefaultsOnly, Category = "Control")
	bool bUsePawnControlRotation;
	UPROPERTY(EditDefaultsOnly, Category = "Control")
	bool bOrientRotationToMovement;
	/** If true, skips all interpolation and puts camera in ideal location.  Automatically set to false next frame. */
	UPROPERTY(transient)
	uint32 bResetInterpolation:1;

public:
	UCameraMode();

	UPlayerCameraComponent* GetCameraComponent() const;

	virtual UWorld* GetWorld() const override;

	// Called when this camera mode is activated on the camera mode stack.
	virtual void OnActivation();
	// Called when this camera mode is deactivated on the camera mode stack.
	virtual void OnDeactivation();

	void UpdateCameraMode(float DeltaTime);

	void SetBlendWeight(float Weight);
	
	AActor* GetTargetActor() const;
	const FCameraModeView& GetCameraModeView() const { return View; }
	float GetBlendTime() const { return BlendTime; }
	float GetBlendWeight() const { return BlendWeight; }
	FGameplayTag GetCameraTypeTag() const { return CameraTypeTag; }

	virtual void DrawDebug(UCanvas* Canvas) const;

protected:

	virtual FVector GetPivotLocation() const;
	virtual FRotator GetPivotRotation() const;

	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);
};

UCLASS()
class UCameraModeStack : public UObject
{
	GENERATED_BODY()
	
protected:
	bool bIsActive;
	/** 지금까지 생성된 CameraMode 에 대한 인스턴스들 저장 */
	UPROPERTY()
	TArray<TObjectPtr<UCameraMode>> CameraModeInstances;
	/** 현재 CameraModeStack 에서 사용하는 CameraMode 들 */
	UPROPERTY()
	TArray<TObjectPtr<UCameraMode>> CameraModeStack;

public:
	UCameraModeStack();

	void ActivateStack();
	void DeactivateStack();

	bool IsStackActivate() const { return bIsActive; }

	void PushCameraMode(TSubclassOf<UCameraMode> CameraModeClass);

	bool EvaluateStack(float DeltaTime, FCameraModeView& OutCameraModeView);

	void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:
	UCameraMode* GetCameraModeInstance(TSubclassOf<UCameraMode> CameraModeClass);

	void UpdateStack(float DeltaTime);
	void BlendStack(FCameraModeView& OutCameraModeView) const;
};
