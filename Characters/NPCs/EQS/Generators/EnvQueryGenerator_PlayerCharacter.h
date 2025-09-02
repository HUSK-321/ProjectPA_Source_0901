// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "EnvQueryGenerator_PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPA_API UEnvQueryGenerator_PlayerCharacter : public UEnvQueryGenerator
{
	GENERATED_BODY()

public:
	UEnvQueryGenerator_PlayerCharacter();

	//~Begin UEnvQueryGenerator interfaces
	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
	//~End UEnvQueryGenerator interfaces
};
