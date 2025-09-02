// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "PAGameplayAbility.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "ParkourAbility.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EParkourType : uint8
{
	Ledge,
	Window,
	Vault,
	Low,

	MAX,
};

/** 파쿠르 어빌리티 사용 시 알아낸 기본 정보들 */
USTRUCT(BlueprintType, Blueprintable)
struct FParkourSearchInfo
{
	GENERATED_BODY()
	
	/** 파쿠르 할 지점 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ParkourLocation;
	/** 파쿠르 할 지점의 Normal */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector ParkourNormal;
	/** 파쿠르 할 지점 뒤의 Floor 가 있다면 그 위치 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector BackFloorLocation;
	/** 파쿠르 할 물체 Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPrimitiveComponent> ParkourTargetComponent;
	/** 장애물의 높이 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ObstacleHeight;
	/** 파쿠르 직전 움직이는 속도 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MoveSpeed;
	/** 파쿠르 Type */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EParkourType ParkourType;
	/** 파쿠르 직전의 MovementMode */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EMovementMode> MovementMode;
	/** 파쿠르 실행 후 뒤에 파쿠르 한 물체가 이어져 있는지 여부 (파쿠르 후 떨어지는지 여부) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHaveBackFloor;
	/** 파쿠르로 사용할 몽타주 (From MotionMatching) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> ParkourMontage;

	FParkourSearchInfo()
		: ParkourLocation(FVector::ZeroVector)
		, ParkourNormal(FVector::ZeroVector)
		, BackFloorLocation(FVector::ZeroVector)
		, ObstacleHeight(0.0f)
		, MoveSpeed(0.0f)
		, ParkourType(EParkourType::MAX)
		, MovementMode(MOVE_None)
		, bHaveBackFloor(false)
	{
	}
};

/**
 * 
 */
UCLASS()
class PROJECTPA_API UParkourAbility : public UPAGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ParkourMontage;
	UPROPERTY(EditAnywhere)
	FName ParkourWarpKey = TEXT("FrontLedge");
	UPROPERTY(EditAnywhere)
	FName ParkourWarpBackFloorKey = TEXT("BackFloor");
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> ParkourLineTraceChannel;
	UPROPERTY(EditAnywhere)
	float ParkourTraceDistance = 200.0f;
	/** 파쿠르 진행 시 손을 짚을 곳을 탐색할때 사용되는 앞 거리 */
	UPROPERTY(EditAnywhere)
	float ParkourHandFrontOffset = 5.0f;

	UPROPERTY(EditAnywhere)
	float ParkourUpperTraceHeight = 70.0f;
	UPROPERTY(EditAnywhere)
	float ParkourForwardTraceHeight = 20.0f;
	UPROPERTY(EditAnywhere)
	float ParkourLowerTraceHeight = -20.0f;
	UPROPERTY(EditAnywhere)
	float ParkourBackFloorTraceDistance = 100.0f;
	UPROPERTY(EditAnywhere)
	float SwimmingTraceHeightOffset = -40.0f;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug = false;
#endif

	// TODO : 초기화
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	FParkourSearchInfo ParkourSearchInfo;
	
public:
	UParkourAbility();
	
	//~Begin GameplayAbility Interfaces
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End GameplayAbility Interfaces

	bool TryDoParkour();

	/** Evaluate Chooser 의 경우 Blueprint 에서 처리하는것이 깔끔합니다. ParkourSearchInfo 를 BP 에서 참조해 저장합니다. */
	UFUNCTION(BlueprintImplementableEvent)
	TArray<UAnimMontage*> EvaluateParkourChooserTable();

private:
	void ClearParkourSearchInfo();
	void SetParkourSettings(bool bParkour);
	
	bool MakeParkourSearchInfo(const AGameCharacterBase* GameCharacter, const FHitResult& InHitResult, float HeightOffset = 0.0f);

	bool DoTraceForwardCharacter(const AGameCharacterBase* GameCharacter, OUT FHitResult& OutHitResult, float HeightOffset) const;

	UFUNCTION()
	void OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted);
	
#if WITH_EDITORONLY_DATA
	void DrawParkourTrace(UWorld* World, const FVector& Start, const FVector& End, const bool bHit, const FVector& ImpactPoint) const;
#endif
};
