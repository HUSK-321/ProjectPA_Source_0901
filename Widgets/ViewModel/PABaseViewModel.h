// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "PABaseViewModel.generated.h"

UCLASS(BlueprintType, Abstract)
class PROJECTPA_API UPABaseViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

protected:
	FName DefaultModelName = TEXT("DefaultModel");

public:
	virtual void Initialize(UObject* OwningObject);
	
	FORCEINLINE FName GetDefaultModelName() const { return DefaultModelName; }

protected:
	APlayerController* GetOwningPlayer() const;
	
	template <typename T = APawn>
	T* GetOwningPawn() const
	{
		if (const APlayerController* PC = GetOwningPlayer())
		{
			return Cast<T>(PC->GetPawn());
		}
		return nullptr;
	}
};
