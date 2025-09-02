// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Widgets/PAWidget.h"
#include "AnnounceMessageWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

/**
 * 게임 내 시스템 메세지를 위한 안내 위젯
 */
UCLASS()
class PROJECTPA_API UAnnounceMessageWidget : public UPAWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AnnounceText;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim, AllowPrivateAccess = true))
	TObjectPtr<UWidgetAnimation> SetVisible;

	FTimerHandle AnnounceHandle;

public:
	UFUNCTION(BlueprintCallable)
	void AddAnnounce(const FString& InMessage);
	void AddAnnounce(const FText& InMessage);

private:
	void ClearAnnounce();
};
