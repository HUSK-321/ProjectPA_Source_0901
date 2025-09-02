// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FootstepAssetBank.generated.h"

class UNiagaraSystem;

/**
 * 캐릭터 애니메이션에 동반되는 사운드를 관리하는 데이터 에셋입니다.
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTPA_API UFootstepAssetBank : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = Audio)
	TMap<FGameplayTag, USoundBase*> SoundBank;
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TMap<TEnumAsByte<EPhysicalSurface>, UNiagaraSystem*> NiagaraFXBank;
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TSet<FGameplayTag> TagsToPlayNiagara;

public:
	USoundBase* GetSound(const FGameplayTag& Tag) const { return SoundBank.FindRef(Tag); }
	UNiagaraSystem* GetNiagaraFX(const FGameplayTag& Tag, const EPhysicalSurface& Surface) const
	{
		return (TagsToPlayNiagara.Contains(Tag)) ? NiagaraFXBank.FindRef(Surface) : nullptr;
	}
};
