// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "PAInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTPA_API UPAInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UPAInputComponent();

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(UInputAction* InputAction, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, FGameplayTag InputTag);
	
protected:
	virtual void BeginPlay() override;
	
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UPAInputComponent::BindAbilityActions(UInputAction* InputAction, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, FGameplayTag InputTag)
{
	if (IsValid(InputAction) && InputTag.IsValid())
	{
		if (PressedFunc)
		{
			BindAction(InputAction, ETriggerEvent::Started, Object, PressedFunc, InputTag);
		}

		if (ReleasedFunc)
		{
			BindAction(InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputTag);
		}
	}
}
