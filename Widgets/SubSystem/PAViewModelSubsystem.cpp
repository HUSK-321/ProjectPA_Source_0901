// All copyrights for this code are owned by Aster.


#include "PAViewModelSubsystem.h"
#include "ProjectPA/Widgets/ViewModel/PABaseViewModel.h"

bool FViewModelKey::operator==(const FViewModelKey& Other) const
{
	return Name == Other.Name && Class == Other.Class;
}

bool FViewModelKey::IsValid() const
{
	return !Name.IsNone() && Class != nullptr;
}

void UPAViewModelSubsystem::Deinitialize()
{
	PendingCallbacks.Empty();
	ViewModelCollection.Empty();
	
	Super::Deinitialize();
}

void UPAViewModelSubsystem::AddViewModel(const FViewModelKey& ViewModelKey, UPABaseViewModel* ViewModel)
{
	if (ViewModelKey.IsValid() == false || IsValid(ViewModel) == false)
	{
		return;
	}

	ViewModelCollection.Add(ViewModelKey, ViewModel);

	if (FOnViewModelReady* FoundDelegate = PendingCallbacks.Find(ViewModelKey))
	{
		FoundDelegate->Broadcast(ViewModel);
		PendingCallbacks.Remove(ViewModelKey);
	}
}

void UPAViewModelSubsystem::BindCallbackOnViewModelAdded(const FViewModelKey& ViewModelKey,
	const FOnViewModelReady::FDelegate& Callback)
{
	if (ViewModelKey.IsValid() == false || Callback.IsBound() == false)
	{
		return;
	}

	if (TObjectPtr<UPABaseViewModel>* FoundViewModel = ViewModelCollection.Find(ViewModelKey))
	{
		Callback.Execute(*FoundViewModel);
	}
	else
	{
		PendingCallbacks.FindOrAdd(ViewModelKey).Add(Callback);
	}
}
