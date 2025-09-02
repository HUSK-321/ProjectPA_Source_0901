// All copyrights for this code are owned by Aster.


#include "PAViewModelUtils.h"
#include "ProjectPA/Widgets/ViewModel/PABaseViewModel.h"

void PAViewModelUtils::AddViewModel(const UObject* WorldContextObject, UPABaseViewModel* ViewModel)
{
	if (IsValid(WorldContextObject) == false || IsValid(WorldContextObject->GetWorld()) == false || IsValid(ViewModel) == false)
	{
		return;
	}

	if (UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		if (UPAViewModelSubsystem* ViewModelSubsystem = GameInstance->GetSubsystem<UPAViewModelSubsystem>())
		{
			FViewModelKey ViewModelKey(ViewModel->GetDefaultModelName(), ViewModel->GetClass());
			ViewModelSubsystem->AddViewModel(ViewModelKey, ViewModel);
		}
	}
}

void PAViewModelUtils::AddViewModel(const UObject* WorldContextObject, const FViewModelKey& ViewModelKey,
                                    UPABaseViewModel* ViewModel)
{
	if (IsValid(WorldContextObject) == false || IsValid(WorldContextObject->GetWorld()) == false || IsValid(ViewModel) == false || ViewModelKey.IsValid() == false)
	{
		return;
	}

	if (UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		if (UPAViewModelSubsystem* ViewModelSubsystem = GameInstance->GetSubsystem<UPAViewModelSubsystem>())
		{
			ViewModelSubsystem->AddViewModel(ViewModelKey, ViewModel);
		}
	}
}

void PAViewModelUtils::BindCallbackOnViewModelAdded(const UObject* WorldContextObject,
	const FViewModelKey& ViewModelKey, const FOnViewModelReady::FDelegate& Callback)
{
	if (IsValid(WorldContextObject) == false || IsValid(WorldContextObject->GetWorld()) == false || ViewModelKey.IsValid() == false)
	{
		return;
	}
	
	if (UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		if (UPAViewModelSubsystem* ViewModelSubsystem = GameInstance->GetSubsystem<UPAViewModelSubsystem>())
		{
			ViewModelSubsystem->BindCallbackOnViewModelAdded(ViewModelKey, Callback);
		}
	}
}
