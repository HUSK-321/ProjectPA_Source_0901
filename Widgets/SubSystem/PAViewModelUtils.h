// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "PAViewModelSubsystem.h"

class UObject;
class UPABaseViewModel;
struct FViewModelKey;

/**
 * ViewModel 관련 C++ 전용 유틸리티 함수들을 모아둔 네임스페이스
 */
namespace PAViewModelUtils
{
	/**
	 * PAViewModelSubsystem에 ViewModel을 편리하게 등록하는 헬퍼 함수
	 */
	void AddViewModel(const UObject* WorldContextObject, UPABaseViewModel* ViewModel);

	void AddViewModel(const UObject* WorldContextObject, const FViewModelKey& ViewModelKey, UPABaseViewModel* ViewModel);

	/**
	 * PAViewModelSubsystem에 ViewModel할당에 대한 콜백을 등록하는 헬퍼 함수
	 */
	void BindCallbackOnViewModelAdded(const UObject* WorldContextObject, const FViewModelKey& ViewModelKey, const FOnViewModelReady::FDelegate& Callback);

	template<CViewModel TViewModel>
	void BindCallbackOnViewModelAdded(const UObject* WorldContextObject, TFunction<void(TViewModel*)>&& Callback)
	{
		if (const TViewModel* ViewModelCDO = GetDefault<TViewModel>())
		{
			BindViewModelByName<TViewModel>(WorldContextObject, ViewModelCDO->GetDefaultModelName(), MoveTemp(Callback));
		}
	}
	
	template<CViewModel TViewModel>
	void BindViewModelByName(const UObject* WorldContextObject, FName ViewModelName, TFunction<void(TViewModel*)>&& Callback)
	{
		if (!IsValid(WorldContextObject) || !IsValid(WorldContextObject->GetWorld()) || !Callback)
		{
			return;
		}

		if (UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
		{
			if (UPAViewModelSubsystem* ViewModelSubsystem = GameInstance->GetSubsystem<UPAViewModelSubsystem>())
			{
				FViewModelKey ViewModelKey{ ViewModelName, TViewModel::StaticClass() };

				FOnViewModelReady::FDelegate Delegate = FOnViewModelReady::FDelegate::CreateLambda(
					[Callback = MoveTemp(Callback)](UPABaseViewModel* BaseViewModel)
					{
						if (TViewModel* ViewModel = Cast<TViewModel>(BaseViewModel))
						{
							Callback(ViewModel);
						}
					});
				
				ViewModelSubsystem->BindCallbackOnViewModelAdded(ViewModelKey, Delegate);
			}
		}
	}
}