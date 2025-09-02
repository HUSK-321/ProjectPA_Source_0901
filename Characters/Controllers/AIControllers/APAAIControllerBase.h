#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "APAAIControllerBase.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UBlackboardComponent;
class UBehaviorTreeComponent;

UENUM(BlueprintType)
enum class EAISenseType : uint8
{
	Sight	UMETA(DisplayName = "Sight"),
	Hearing UMETA(DisplayName = "Hearing"),
	Damage	UMETA(DisplayName = "Damage"),
	Team	UMETA(DisplayName = "Team"),
	Touch	UMETA(DisplayName = "Touch"),
	MAX		UMETA(DisplayName = "MAX") 
};

UCLASS()
class PROJECTPA_API APAAIControllerBase : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess=true))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess=true))
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess=true))
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess=true))
	FName TargetActorBlackboardKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess=true))
	float SenseSoundActiveTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess=true))
	FName SenseSoundBlackboardKey;
	FTimerHandle SenseSoundTimer;
	
public:
	APAAIControllerBase();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual bool HandleSightStimulus(AActor* SensedActor, const FAIStimulus& Stimulus);
	virtual bool HandleHearingStimulus(AActor* SensedActor, const FAIStimulus& Stimulus);
	virtual bool HandleDamageStimulus(AActor* SensedActor, const FAIStimulus& Stimulus);

	void ClearSenseSound();

private:
	UFUNCTION()
	void AIPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	void OnPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
