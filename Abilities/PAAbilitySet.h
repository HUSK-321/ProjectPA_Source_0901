#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ProjectPA/Components/CharacterAbilitySystemComponent.h"
#include "PAAbilitySet.generated.h"

class UAbilitySystemComponent;
class UPAGameplayAbility;
class UInputAction;

USTRUCT()
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPAGameplayAbility> AbilityClass;
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel;
};

USTRUCT()
struct FAbilityInfoWithInput : public FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Input"))
	FGameplayTag InputTag;
};

UCLASS()
class PROJECTPA_API UPAAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Ability")
	TArray<FAbilityInfo> AbilityInfos;
	UPROPERTY(EditAnywhere, Category="Ability")
	TArray<FAbilityInfoWithInput> AbilityInfosWithInput;

public:
	void GiveAbilityToTarget(UCharacterAbilitySystemComponent* ASC, UObject* SourceObject) const;
};
