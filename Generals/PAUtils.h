#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "UObject/Object.h"

namespace UPAUtils
{
	static float GetNotifyTimeFromMontage(UAnimMontage* Montage, FName NotifyName, bool bReverse = false)
	{
		if (Montage == nullptr)
		{
			return -1.0f;
		}

		for (const FAnimNotifyEvent& NotifyEvent : Montage->Notifies)
		{
			if (NotifyEvent.NotifyName == NotifyName)
			{
				return bReverse ? Montage->GetPlayLength() - NotifyEvent.GetTriggerTime() : NotifyEvent.GetTriggerTime();
			}
		}

		return -1.0f;
	}

	/**
	 * @brief TSubclassOf 를 찾게 해주는 템플릿 함수
	 * @tparam T UUserWidget 파생의 위젯 클래스
	 * @param Path 해당 위젯 클래스의 BP 경로
	 */
	template <typename T>
	typename TEnableIf<TIsDerivedFrom<T, UUserWidget>::Value, TSubclassOf<T>>::Type
	FindWidgetClass(const FString& Path)
	{
		ConstructorHelpers::FClassFinder<T> WidgetClassFinder(*Path);
		if (WidgetClassFinder.Succeeded())
		{
			return WidgetClassFinder.Class;
		}
		return nullptr;
	}
	
	static UDataTable* LoadDataTable(const FString& DataTablePath)
	{
		UObject* LoadedObject = StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath);
		if (UDataTable* LoadedDataTable = Cast<UDataTable>(LoadedObject))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Table Loaded Successfully : %s"), *DataTablePath);
			return LoadedDataTable;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load Data Table at path: %s"), *DataTablePath);
			return nullptr;
		}
	}

	static UDataAsset* LoadDataAsset(const FString& DataAssetPath)
	{
		UObject* LoadedObject = StaticLoadObject(UDataAsset::StaticClass(), nullptr, *DataAssetPath);
    
		if (UDataAsset* LoadedDataAsset = Cast<UDataAsset>(LoadedObject))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Data Asset Loaded Successfully: %s"), *DataAssetPath);
			return LoadedDataAsset;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load Data Asset at path: %s"), *DataAssetPath);
			return nullptr;
		}
	}
};