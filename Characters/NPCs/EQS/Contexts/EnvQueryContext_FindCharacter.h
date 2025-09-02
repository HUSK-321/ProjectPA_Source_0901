// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_FindCharacter.generated.h"

class AGameCharacterBase;

UCLASS(Blueprintable)
class PROJECTPA_API UEnvQueryContext_FindCharacter : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	//~Begin UEnvQueryContext interface
	virtual void ProvideContext(FEnvQueryInstance& Instance, FEnvQueryContextData& ContextData) const override;
	//~End UEnvQueryContext interface

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameCharacterBase> SearchCharacterClass;
};
