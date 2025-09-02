// All copyrights for this code are owned by Aster.


#include "PACheatManager.h"
#include "AbilitySystemComponent.h"

void UPACheatManager::GiveStatusEffect(const FString& StatusEffect)
{
	if (GetOuterAPlayerController()->HasAuthority() == false)
	{
		ServerGiveStatusEffect(StatusEffect);
		return;
	}
	ApplyStatusEffect(StatusEffect);
}

void UPACheatManager::ApplyStatusEffect(const FString& StatusEffect)
{
	if (CheatEffectList.Find(StatusEffect) == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CheatEffectList 확인점"));
		return;
	}

	const TSubclassOf<UGameplayEffect> CheatGameplayEffect = CheatEffectList[StatusEffect];
	if (APlayerController* PC = GetOuterAPlayerController())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			if (UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>())
			{
				if (CheatGameplayEffect)
				{
					const FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
					const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CheatGameplayEffect, 1.0f, EffectContext);
					if (SpecHandle.IsValid())
					{
						ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
						UE_LOG(LogTemp, Warning, TEXT("Cheat GameplayEffect 적용됨!"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("GameplayEffect Spec 생성 실패"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("CheatGameplayEffect가 설정되지 않음"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent를 찾을 수 없음"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn을 찾을 수 없음"));
		}
	}
}

void UPACheatManager::ServerGiveStatusEffect_Implementation(const FString& StatusEffect)
{
	ApplyStatusEffect(StatusEffect);
}
