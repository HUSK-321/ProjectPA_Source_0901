// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Characters/NPCs/PersonalityTraitSystem/PersonalityTraitTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PersonalitySubsystem.generated.h"

class UBehaviorTree;
class UPersonalityCalculator;

/**
 * 
 */
UCLASS()
class PROJECTPA_API UPersonalitySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> DefaultBehaviorTree;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> ActionTypeBehaviorTrees[static_cast<int8>(EPersonalityActionType::MAX)];
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPersonalityCalculator> PersonalityCalculatorClass;
	UPROPERTY(Transient)
	TObjectPtr<UPersonalityCalculator> PersonalityCalculator;
	
public:
	//~Begin UGameInstanceSubsystem Functions
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~End UGameInstanceSubsystem Functions
	
	UBehaviorTree* GetBehaviorTreeForWeights(const FActionWeightsArray& ActionWeights) const;
	UBehaviorTree* GetDefaultBehaviorTree() const { return DefaultBehaviorTree; }
};
