#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "BagItemBase.generated.h"

enum class EGetItemError : uint8;

UCLASS()
class PROJECTPA_API UBagItemBase : public UItemBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<FItemEncodedInfo> ItemList;
	
	/** 해당 인벤토리 컴포넌트가 최대로 가질 수 있는 무게 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float MaxWeight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float CurrentWeight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<EItemType> CannotContainTypes;

public:
	UBagItemBase();

	virtual void InitializeItemInfo(const FItemDataTable* DataTableInfo) override;

	void GetItemList(OUT TArray<FItemEncodedInfo>& OutItemList) const;
	FItemEncodedInfo* GetItem(const FItemEncodedInfo& InItemEncodedInfo);
	EGetItemError CanGetItem(const FItemEncodedInfo& InItem);
	EGetItemError TryAddItem(const FItemEncodedInfo& InItem);

	bool TryToRemoveItem(const FItemEncodedInfo& InItem);

	bool HaveItem(const FName& ItemKeyToFind);
	// TODO : Current Weight 계산 방식을 테이블 기반으로 변경
	FORCEINLINE float GetItemsWeight() const { return CurrentWeight + GetWeight(); }

};
