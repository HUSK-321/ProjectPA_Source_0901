// All copyrights for this code are owned by Aster.


#include "PersonalitySubsystem.h"
#include "ProjectPA/Characters/NPCs/PersonalityTraitSystem/PersonalityTraitCalculators/PersonalityCalculator.h"

void UPersonalitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//static ConstructorHelpers::FClassFinder<UPersonalityCalculator> CalculatorClassFinder(TEXT("/Game/01_GameCharacters/NPCs/PersonalityTraits/Calculator/BP_DefaultPersonalityCalculator"));
    
	// if (CalculatorClassFinder.Succeeded())
	// {
	// 	PersonalityCalculatorClass = CalculatorClassFinder.Class;
	// 	if (PersonalityCalculatorClass)
	// 	{
	// 		PersonalityCalculator = NewObject<UPersonalityCalculator>(this, PersonalityCalculatorClass);
	// 	}
	// }
	
	Super::Initialize(Collection);
}

void UPersonalitySubsystem::Deinitialize()
{
	PersonalityCalculator = nullptr;
	
	Super::Deinitialize();
}

UBehaviorTree* UPersonalitySubsystem::GetBehaviorTreeForWeights(const FActionWeightsArray& ActionWeights) const
{
	UBehaviorTree* SelectedTree = DefaultBehaviorTree;
	int32 MaxWeight = INDEX_NONE;
	int32 MaxIndex = INDEX_NONE;

	for (int32 i = 0; i < ActionWeights.Weights.Num(); ++i)
	{
		if (ActionWeights.Weights[i] > MaxWeight)
		{
			MaxWeight = ActionWeights.Weights[i];
			MaxIndex = i;
		}
	}
    
	constexpr int32 ActionThreshold = 50;
	if (MaxWeight < ActionThreshold || MaxIndex == INDEX_NONE)
	{
		return SelectedTree;
	}

	SelectedTree = ActionTypeBehaviorTrees[MaxIndex] != nullptr ? ActionTypeBehaviorTrees[MaxIndex] : DefaultBehaviorTree;
	return SelectedTree;
}
