// All copyrights for this code are owned by Aster.


#include "AnimNotifyState_ParkourMontageBlendOut.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_ParkourMontageBlendOut::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                         float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (IsValid(MeshComp) == false)
	{
		return;
	}
	
	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance) == false)
	{
		return;
	}
	
	bool bShouldBlendOut = false;
	switch (ParkourBlendOutCondition)
	{
	case EParkourBlendOutCondition::ForceBlendOut:
		bShouldBlendOut = true;
		break;
	case EParkourBlendOutCondition::WithMovementInput:
		bShouldBlendOut = IsAccelerationIsZero(Cast<ACharacter>(MeshComp->GetOwner())) == false;
		break;
	case EParkourBlendOutCondition::IfFalling:
		bShouldBlendOut = IsCharacterFalling(Cast<ACharacter>(MeshComp->GetOwner()));
		break;
	}

	if (bShouldBlendOut == false)
	{
		return;
	}

	UAnimMontage* ParkourMontage = Cast<UAnimMontage>(Animation);
	if (IsValid(ParkourMontage))
	{
		FAlphaBlendArgs AlphaBlend(BlendOutTime);
		AlphaBlend.BlendOption = EAlphaBlendOption::HermiteCubic;
		
		FMontageBlendSettings BlendSettings(AlphaBlend);
		BlendSettings.BlendProfile = const_cast<UBlendProfile*>(AnimInstance->GetBlendProfileByName(BlendProfileName));
		BlendSettings.BlendMode = EMontageBlendMode::Standard;
		
		AnimInstance->Montage_StopWithBlendSettings(BlendSettings, ParkourMontage);	
	}
}

FString UAnimNotifyState_ParkourMontageBlendOut::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("Parkour BlendOut %s"), *UEnum::GetValueAsString(ParkourBlendOutCondition));
}

bool UAnimNotifyState_ParkourMontageBlendOut::IsAccelerationIsZero(const ACharacter* Character) const
{
	if (IsValid(Character) == false)
	{
		return true;
	}
	 
	if (UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement())
	{
		return CharacterMovement->GetCurrentAcceleration().IsNearlyZero(0.1f);
	}
	return true;
}

bool UAnimNotifyState_ParkourMontageBlendOut::IsCharacterFalling(const ACharacter* Character) const
{
	if (IsValid(Character) == false)
	{
		return false;
	}
	
	if (UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement())
	{
		return CharacterMovement->IsFalling();
	}
	return false;
}
