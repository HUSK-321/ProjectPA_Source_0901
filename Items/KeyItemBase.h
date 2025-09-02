// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "KeyItemBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UKeyItemBase : public UItemBase
{
	GENERATED_BODY()

private:
	/** 각 열쇠 아이템을 구분짓기 위한 KeyCode */
	UPROPERTY(EditAnywhere)
	int32 KeyCode;

public:
	virtual void InitializeItemInfo(const FItemDataTable* DataTableInfo) override;
	bool IsKeyCorrect(const int32 InKeyCode) { return KeyCode == InKeyCode; }
};
