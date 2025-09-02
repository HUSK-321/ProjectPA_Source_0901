// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "ItemGroupButtonWidget.generated.h"

class UImage;
class UButton;

DECLARE_DELEGATE_OneParam(FOnItemGroupButtonClicked, const int32);

/**
 * 플레이어 아이템 위젯에서 위 아이템 그룹을 나타낼 버튼. 클릭 시 해당 그룹의 아이템들을 보여줍니다
 */
UCLASS()
class PROJECTPA_API UItemGroupButtonWidget : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ButtonIcon;
	
	int32 GroupIndex;
	FOnItemGroupButtonClicked OnButtonClickedDelegate;
	
public:
	UFUNCTION()
	void OnButtonClicked();
	
	void SetButtonClickedEvent(const int32 ButtonIndex, const FOnItemGroupButtonClicked& OnClickedCallback, UTexture2D* InButtonIcon = nullptr); 
};
