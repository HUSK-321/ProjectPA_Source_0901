#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayGameHud.generated.h"

class UPlayerWidget;
class UAnnounceMessageWidget;
class UEnvironmentStatusWidget;
class UPlayerStatusWidget;

UCLASS()
class PROJECTPA_API APlayGameHud : public AHUD
{
	GENERATED_BODY()

private:
	TSubclassOf<UPlayerWidget> PlayerWidgetClass;
	UPROPERTY()
	TObjectPtr<UPlayerWidget> PlayerWidget;
	UPROPERTY()
	TObjectPtr<UEnvironmentStatusWidget> EnvironmentStatusWidget;
	UPROPERTY()
	TObjectPtr<UPlayerStatusWidget> StatusWidget;
	
	TSubclassOf<UAnnounceMessageWidget> AnnounceMessageWidgetClass;
	UPROPERTY()
	TObjectPtr<UAnnounceMessageWidget> AnnounceMessageWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerInventoryWidget> PlayerInventoryWidgetClass;
	UPROPERTY()
	TObjectPtr<UPlayerInventoryWidget> PlayerInventoryWidget;

public:
	APlayGameHud();

	UFUNCTION(BlueprintCallable)
	void SetAnnounceMessage(const FString& InMessage);

	void ToggleInventoryWidget();
	
	// TODO : Remove and Set binding here
	FORCEINLINE UPlayerStatusWidget* GetStatusWidget() { return StatusWidget; }

protected:
	virtual void BeginPlay() override;
};
