#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IngamePlayerController.generated.h"

UCLASS()
class PROJECTPA_API AIngamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AIngamePlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
