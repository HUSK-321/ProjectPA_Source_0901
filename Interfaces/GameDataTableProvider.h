// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectPA/Generals/PATypes.h"
#include "GameDataTableProvider.generated.h"

class UItemBase;
struct FItemDataTable;

UINTERFACE()
class UGameDataTableProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * GameInstance 에서 DataTable 함수만을 분리한 경우입니다
 */
class PROJECTPA_API IGameDataTableProvider
{
	GENERATED_BODY()

public:
	virtual FItemDataTable* GetItemDataByKey(const FName& ItemTableKey) const { return nullptr; }
	virtual UItemBase* GetItemByKey(const FName& ItemTableKey, UObject* ItemOuter = nullptr, const FItemEncodedInfo& ItemEncodedInfo = FItemEncodedInfo()) { return nullptr; }
	template <typename T>
	T* GetItemByKey(const FName& ItemTableKey, UObject* ItemOuter = nullptr, const FItemEncodedInfo& ItemEncodedInfo = FItemEncodedInfo())
	{
		return Cast<T>(GetItemByKey(ItemTableKey, ItemOuter, ItemEncodedInfo));
	}
};
