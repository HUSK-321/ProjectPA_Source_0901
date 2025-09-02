// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "StatusGameplayEffect.generated.h"

USTRUCT()
struct FEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute Attribute;
	UPROPERTY(EditDefaultsOnly)
	float Amount;
};

/**
 * 게임 내 버프/디버프 상태이상을 모두 아우르는 클래스입니다.
 */
UCLASS()
class PROJECTPA_API UStatusGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

private:
	/** 상태이상 이름 */
	UPROPERTY(EditDefaultsOnly, Category = "Status Effect")
	FText StatusEffectName;
	/** 상태이상 설명 */
	UPROPERTY(EditDefaultsOnly, Category = "Status Effect")
	FText StatusEffectDescription;
	/** 상태이상에 대응하는 아이콘 */
	UPROPERTY(EditDefaultsOnly, Category = "Status Effect")
	TSoftObjectPtr<UTexture2D> StatusEffectIcon;
	/** 해당 상태이상이 속하는 카테고리 */
	UPROPERTY(EditDefaultsOnly, Category = "Status Effect")
	FGameplayTag StatusEffectTag;
	/** 해당 카테고리 안에서 레벨 */
	UPROPERTY(EditDefaultsOnly, Category = "Status Effect")
	int32 EffectLevel;
	/** 해당 이펙트가 처음 적용될 때 들어갈 수치 */
	UPROPERTY(EditDefaultsOnly, Category = "Status Effect")
	TArray<FEffectInfo> Effects;
	/** 해당 이펙트가 있을 때 게임 내 10분마다 적용할 수치 */
	UPROPERTY(EditDefaultsOnly, Category = "Status Effect")
	TArray<FEffectInfo> PeriodicEffects;

public:

	
	FORCEINLINE const FGameplayTag& GetStatusCategory() const { return StatusEffectTag; }
	FORCEINLINE int32 GetEffectLevel() const { return EffectLevel; }
	UTexture2D* GetIcon() const;
	void ApplyPeriodicEffects(UAbilitySystemComponent* AbilitySystemComponent) const;
};
