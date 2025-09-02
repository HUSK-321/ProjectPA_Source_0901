// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectPA/Generals/PATypes.h"
#include "ProjectPA/Interfaces/GameDataTableProvider.h"
#include "PAGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FItemClassAndDataTable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemBase> ItemClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> DataTable;
};

/**
 * 
 */
UCLASS()
class PROJECTPA_API UPAGameInstance : public UGameInstance, public IGameDataTableProvider
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Item Data", meta = (AllowPrivateAccess = true))
	TMap<FString, FItemClassAndDataTable> ItemDataTableMap;

public:
	virtual void Init() override;

	virtual FItemDataTable* GetItemDataByKey(const FName& ItemTableKey) const override;
	virtual UItemBase* GetItemByKey(const FName& ItemTableKey, UObject* ItemOuter = nullptr, const FItemEncodedInfo& ItemEncodedInfo = FItemEncodedInfo()) override;

private:
	const FItemClassAndDataTable* GetItemClassAndDataTable(const FName& ItemTableKey) const;
};
