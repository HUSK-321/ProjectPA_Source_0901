// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectPA/Interfaces/PAActor.h"
#include "PAActorBase.generated.h"

UCLASS()
class PROJECTPA_API APAActorBase : public AActor, public IPAActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_UID)
	int32 ActorUID;
	
public:
	APAActorBase();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	virtual void Tick(float DeltaTime) override;

	//~Begin IPAActor Interface
	virtual void RequestUID() override;
	virtual EUIDType GetContainingType() const override;
	virtual int32 GetUID() const override { return ActorUID; }
	virtual void SetUID(int32 NewUID) override;
	UFUNCTION()
	virtual void OnRep_UID() override;
	//~End IPAActor Interface
};
