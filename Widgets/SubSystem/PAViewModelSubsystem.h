// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PAViewModelSubsystem.generated.h"

class UPABaseViewModel;

/** ViewModel이 준비되었을 때 브로드캐스트될 멀티캐스트 델리게이트입니다. */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnViewModelReady, UPABaseViewModel* /*ViewModel*/);

/** ViewModel을 식별하기 위한 복합 키 구조체입니다. */
USTRUCT()
struct FViewModelKey
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY()
	TSubclassOf<UPABaseViewModel> Class;

	bool operator==(const FViewModelKey& Other) const;
	bool IsValid() const;

	friend uint32 GetTypeHash(const FViewModelKey& Key)
	{
		return HashCombine(GetTypeHash(Key.Name), GetTypeHash(Key.Class));
	}
};

template<typename T>
concept CViewModel = TIsDerivedFrom<T, UPABaseViewModel>::IsDerived;

/**
 * 프로젝트의 ViewModel을 중앙에서 관리하고, 등록을 구독하는 기능을 제공하는 독립적인 서브시스템입니다.
 */
UCLASS()
class PROJECTPA_API UPAViewModelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	/** 지정된 키로 ViewModel을 등록하고, 이 키를 기다리던 모든 콜백을 실행(브로드캐스트)합니다. */
	void AddViewModel(const FViewModelKey& ViewModelKey, UPABaseViewModel* ViewModel);

	/** 특정 ViewModel이 준비되면 호출될 콜백을 등록(구독)합니다. */
	void BindCallbackOnViewModelAdded(const FViewModelKey& ViewModelKey, const FOnViewModelReady::FDelegate& Callback);
	
	template<CViewModel TViewModel>
	TViewModel* GetViewModel(const FViewModelKey& ViewModelKey)
	{
		return (ViewModelCollection.Contains(ViewModelKey)) ? ViewModelCollection[ViewModelKey] : nullptr;
	}

private:
	UPROPERTY(Transient)
	TMap<FViewModelKey, TObjectPtr<UPABaseViewModel>> ViewModelCollection;

	TMap<FViewModelKey, FOnViewModelReady> PendingCallbacks;
};