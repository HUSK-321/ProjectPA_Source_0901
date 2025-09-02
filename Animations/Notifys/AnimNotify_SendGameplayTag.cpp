// All copyrights for this code are owned by Aster.

#include "AnimNotify_SendGameplayTag.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotify_SendGameplayTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr || MeshComp->GetWorld() == nullptr || MeshComp->GetWorld()->IsGameWorld() == false)
	{
		return;
	}

	if (MeshComp->GetOwner())
	{
		FGameplayEventData GameplayEventData;
		GameplayEventData.EventTag = TagToSend;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), TagToSend, GameplayEventData);
	}
}
