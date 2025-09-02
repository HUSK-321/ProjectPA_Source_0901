// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Generals/PATypes.h"
#include "ProjectPA/InGameActors/PAActorBase.h"
#include "ProjectPA/Interfaces/InteractableActor.h"
#include "BroadcastActorBase.generated.h"

struct FBroadcastDataTable;
class UBroadcastActorWidget;
class UWidgetComponent;
struct FBroadcastTimeDataTable;

DECLARE_DELEGATE_RetVal(const FGameTimeInfo&, FGetGameTimeDelegate);

static const FName NoBroadcastKey = FName(TEXT("NoBroadcast"));

/**
 * 라디오, 텔레비전 등의 UI로 텍스트가 나오는 액터들의 기본 클래스
 */
UCLASS()
class PROJECTPA_API ABroadcastActorBase : public APAActorBase, public IInteractableActor
{
	GENERATED_BODY()

private:
	FGetGameTimeDelegate GetGameTimeDelegate;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ActorMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> BroadcastWidgetComponent;

	/** 방송 시간이 담긴 테이블 */
	UPROPERTY(EditAnywhere, Category = "Radio Data")
	TObjectPtr<UDataTable> BroadcastTimeDataTable;
	/** 현재 방송 시간의 정보 */
	FBroadcastTimeDataTable* CurrentBroadcast;
	/** 방송 내용이 담긴 테이블 */
	UPROPERTY(EditAnywhere, Category = "Radio Data")
	TObjectPtr<UDataTable> BroadcastDataTable;
	UPROPERTY()
	TObjectPtr<UBroadcastActorWidget> BroadcastWidget;

	UPROPERTY(EditAnywhere)
	float BroadcastInterval;
	
	int CurrentDialogueIndex;
	FTimerHandle BroadcastTimer;

public:
	ABroadcastActorBase();

	virtual void InteractWith(APlayerCharacter* InteractingPlayer) override;
	virtual void OnInteractStart(APlayerCharacter* InteractingPlayer) override;
	virtual void OnInteractStop(APlayerCharacter* InteractingPlayer) override;

	virtual void DisplayBroadcast();
	void ReserveNextBroadcast();

protected:
	virtual void BeginPlay() override;

private:
	void UpdateCurrentBroadcast();
	void FindNewBroadcast();
	
	FName GetBroadcastDialogueKey();
	const FBroadcastDataTable* GetBroadcastData(const FName& BroadcastKey) const;
	const FGameTimeInfo& GetGameTime();
	bool IsBroadcastTime();
	
	void SetBroadcastWidget(bool bVisible);
};
