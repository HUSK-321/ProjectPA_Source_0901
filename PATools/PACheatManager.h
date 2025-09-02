// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "PACheatManager.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class PROJECTPA_API UPACheatManager : public UCheatManager
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TMap<FString, TSubclassOf<UGameplayEffect>> CheatEffectList;
	
public:
	UFUNCTION(exec)
	void GiveStatusEffect(const FString& StatusEffect);
	void ApplyStatusEffect(const FString& StatusEffect);
	UFUNCTION(Server, Reliable)
	void ServerGiveStatusEffect(const FString& StatusEffect);
};
