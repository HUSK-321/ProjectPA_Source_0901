#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectPA/Interfaces/StatProviderComponent.h"
#include "StatComponent.generated.h"

/**
 * 플레이어의 능력치를 담당할 클래스 (지식, 운 등의 능력치)
 * TODO : 이후 UHealthComponent와 어떤 식으로 소통할지 결정
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTPA_API UStatComponent : public UActorComponent, public IStatProviderComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

protected:
	virtual void BeginPlay() override;

public:
};
