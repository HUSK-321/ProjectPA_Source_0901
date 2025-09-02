// All copyrights for this code are owned by Aster.


#include "ParkourAbility.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "ProjectPA/PALog.h"

#include "ProjectPA/Characters/GameCharacterBase.h"

UParkourAbility::UParkourAbility()
{
}

bool UParkourAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                         const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return true;
	//return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UParkourAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TryDoParkour() == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UParkourAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	check(ActorInfo);
	if (IsValid(ActorInfo->GetAnimInstance()))
	{
		ActorInfo->GetAnimInstance()->OnMontageEnded.RemoveAll(this);
	}
	SetParkourSettings(false);
	ClearParkourSearchInfo();
}

bool UParkourAbility::TryDoParkour()
{
	AGameCharacterBase* GameCharacter = Cast<AGameCharacterBase>(GetAvatarActorFromActorInfo());
	if (IsValid(GameCharacter) == false || IsValid(GameCharacter->GetCharacterMovement()) == false)
	{
		return false;
	}
	
	UCharacterMovementComponent* CharacterMovement = GameCharacter->GetCharacterMovement();
	ParkourSearchInfo.MovementMode = CharacterMovement->MovementMode;
	ParkourSearchInfo.MoveSpeed = CharacterMovement->Velocity.Size2D();
	ParkourSearchInfo.ParkourType = EParkourType::MAX;
	
	FHitResult UpperHit, FrontHit, LowerHit;
	const bool bUpper = DoTraceForwardCharacter(GameCharacter, UpperHit, ParkourUpperTraceHeight);
	const bool bFront = DoTraceForwardCharacter(GameCharacter, FrontHit, ParkourForwardTraceHeight);
	const bool bLower = DoTraceForwardCharacter(GameCharacter, LowerHit, ParkourLowerTraceHeight);
	
	bool bSuccessNextTrace = false;
	if (bUpper == false && bFront)
	{
		bSuccessNextTrace = MakeParkourSearchInfo(GameCharacter, FrontHit, ParkourUpperTraceHeight);
	}
	else if (bUpper && bLower && bFront == false)
	{
		bSuccessNextTrace = MakeParkourSearchInfo(GameCharacter, LowerHit);
	}
	else if (bFront == false && bLower)
	{
		bSuccessNextTrace = MakeParkourSearchInfo(GameCharacter, LowerHit);
	}

	if (bSuccessNextTrace == false)
	{
		return false;
	}
	
	TArray<UAnimMontage*> SelectedParkourMontages = EvaluateParkourChooserTable();
	if (ParkourSearchInfo.ParkourType != EParkourType::MAX && SelectedParkourMontages.IsEmpty() == false)
	{
		UAnimInstance* AnimInstance = IsValid(GameCharacter->GetMesh()) ? GameCharacter->GetMesh()->GetAnimInstance() : nullptr;
		if (IsValid(AnimInstance) == false)
		{
			return false;
		}
		
		FPoseSearchBlueprintResult PoseSearchResult;
		UPoseSearchLibrary::MotionMatch(AnimInstance, reinterpret_cast<const TArray<UObject*>&>(SelectedParkourMontages), FName("PoseHistory"), FPoseSearchContinuingProperties(), FPoseSearchFutureProperties(), PoseSearchResult);

		const UObject* TargetParkourAnim = IsValid(PoseSearchResult.SelectedAnim) ? PoseSearchResult.SelectedAnim : SelectedParkourMontages[0]; 
		UAnimMontage* SelectedParkourMontage = const_cast<UAnimMontage*>(Cast<UAnimMontage>(TargetParkourAnim));
		if (IsValid(SelectedParkourMontage) == false)
		{
			return false;
		}
		
		UAbilitySystemComponent* ASC = GameCharacter->GetAbilitySystemComponent();
		UMotionWarpingComponent* MotionWarpingComponent = GameCharacter->GetMotionWarpingComponent();
		check(MotionWarpingComponent);
		if (IsValid(ASC))
		{
			SetParkourSettings(true);
			
			const FRotator ParkourRotation = UKismetMathLibrary::FindLookAtRotation(GameCharacter->GetActorLocation(), ParkourSearchInfo.ParkourLocation);
			const FRotator ActorRotation = GameCharacter->GetActorRotation();
			const FRotator TargetRotation{ ActorRotation.Pitch, ParkourRotation.Yaw, ActorRotation.Roll };
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(ParkourWarpKey, ParkourSearchInfo.ParkourLocation, TargetRotation);
			
			DrawDebugPoint(GetWorld(), ParkourSearchInfo.ParkourLocation, 20.0f, FColor::Purple, true);
			ASC->PlayMontage(this, GetCurrentActivationInfo(), SelectedParkourMontage, 1.0f);

			if (GameCharacter->GetMesh() && GameCharacter->GetMesh()->GetAnimInstance())
			{
				GameCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UParkourAbility::OnMontageEndedCallback);
			}
			return true;
		}
	}

	return false;
}

void UParkourAbility::ClearParkourSearchInfo()
{
	ParkourSearchInfo.bHaveBackFloor = false;
	ParkourSearchInfo.BackFloorLocation = FVector::ZeroVector; 
}

void UParkourAbility::SetParkourSettings(bool bParkour)
{
	check(GetAvatarActorFromActorInfo());
	AGameCharacterBase* Character = Cast<AGameCharacterBase>(GetAvatarActorFromActorInfo());
	if (IsValid(Character))
	{
		check(Character->GetCapsuleComponent());
		//Character->GetCapsuleComponent()->IgnoreComponentWhenMoving(ParkourSearchInfo.ParkourTargetComponent, bParkour);
		
		if (IsValid(Character->GetCharacterMovement()))
		{
			UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
			MovementComponent->bIgnoreClientMovementErrorChecksAndCorrection = bParkour;
			MovementComponent->bServerAcceptClientAuthoritativePosition = bParkour;

			if (bParkour)
			{
				MovementComponent->SetMovementMode(MOVE_Flying);	
			}
			else
			{
				EMovementMode NewMovementMode = MovementComponent->MovementMode == MOVE_Swimming ? MOVE_Swimming : MOVE_Walking;
				MovementComponent->SetMovementMode(NewMovementMode);
			}
		}
		
		if (bParkour == false)
		{
			UMotionWarpingComponent* MotionWarpingComponent = Character->GetMotionWarpingComponent();
			if (IsValid(MotionWarpingComponent))
			{
				MotionWarpingComponent->RemoveWarpTarget(ParkourWarpKey);
				MotionWarpingComponent->RemoveWarpTarget(ParkourWarpBackFloorKey);				
			}
		}
	}
}

bool UParkourAbility::MakeParkourSearchInfo(const AGameCharacterBase* GameCharacter, const FHitResult& InHitResult, const float HeightOffset)
{
	check(GameCharacter);
	
	FVector TraceEndLocation = InHitResult.Location + GameCharacter->GetActorForwardVector() * ParkourHandFrontOffset;
	FVector TraceStartLocation = TraceEndLocation + FVector{0.0f, 0.0f, 100.0f + HeightOffset};

	FHitResult HitResult;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ParkourTrace), false);
	QueryParams.AddIgnoredActor(GameCharacter);
	const bool bHit = GameCharacter->GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ParkourLineTraceChannel, QueryParams);
#if WITH_EDITORONLY_DATA
	DrawParkourTrace(GameCharacter->GetWorld(), TraceStartLocation, TraceEndLocation, bHit, HitResult.ImpactPoint);
#endif
	if (bHit)
	{
		ParkourSearchInfo.ParkourTargetComponent = HitResult.GetComponent();
		ParkourSearchInfo.ParkourLocation = HitResult.ImpactPoint;
		ParkourSearchInfo.ParkourNormal = HitResult.ImpactNormal;
		const float CharacterZPos = GameCharacter->GetActorLocation().Z - GameCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		ParkourSearchInfo.ObstacleHeight = FMath::Abs(CharacterZPos - HitResult.ImpactPoint.Z);
		
		// Do Search BackFloor
		{
			const FVector BackFloorDirection = (HitResult.ImpactPoint - GameCharacter->GetActorLocation()).GetSafeNormal2D();
			const FVector BackFloorTraceStart = HitResult.ImpactPoint + BackFloorDirection * ParkourBackFloorTraceDistance + FVector{0.0f, 0.0f, 5.0f}; // Give Z Offset
			const FVector BackFloorTraceEnd = HitResult.ImpactPoint + BackFloorDirection * ParkourBackFloorTraceDistance + FVector{0.0f, 0.0f, -50.0f}; // Give Z Offset

			FHitResult BackFloorHitResult;
			ParkourSearchInfo.bHaveBackFloor = GameCharacter->GetWorld()->LineTraceSingleByChannel(BackFloorHitResult, BackFloorTraceStart, BackFloorTraceEnd, ParkourLineTraceChannel, QueryParams);
			if (ParkourSearchInfo.bHaveBackFloor)
			{
				ParkourSearchInfo.BackFloorLocation = BackFloorHitResult.Location;
			}

#if WITH_EDITORONLY_DATA
			DrawParkourTrace(GameCharacter->GetWorld(), BackFloorTraceStart, BackFloorTraceEnd, ParkourSearchInfo.bHaveBackFloor, BackFloorHitResult.ImpactPoint);
#endif
		}
	}

	return bHit;
	
}

bool UParkourAbility::DoTraceForwardCharacter(const AGameCharacterBase* GameCharacter, OUT FHitResult& OutHitResult, float HeightOffset) const
{
	if (IsValid(GameCharacter) == false)
	{
		return false;
	}
	
	const float CharacterRadius = GameCharacter->GetCapsuleComponent() ? GameCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() : 40.0f;
	const FVector ForwardVector = GameCharacter->GetActorForwardVector();
	const FVector TraceStartLocation = GameCharacter->GetActorLocation() + (ForwardVector * CharacterRadius) + FVector{0.0f, 0.0f, HeightOffset};
	const FVector TraceEndLocation = TraceStartLocation + (ForwardVector * ParkourTraceDistance);
	
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ParkourTrace), false);
	QueryParams.AddIgnoredActor(GameCharacter);
	const bool bHit = GameCharacter->GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStartLocation, TraceEndLocation, ParkourLineTraceChannel, QueryParams);
#if WITH_EDITORONLY_DATA
	DrawParkourTrace(GameCharacter->GetWorld(), TraceStartLocation, TraceEndLocation, bHit, OutHitResult.ImpactPoint);
#endif
	
	return bHit;
}

void UParkourAbility::OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted)
{
	K2_EndAbility();
}

#if WITH_EDITORONLY_DATA
void UParkourAbility::DrawParkourTrace(UWorld* World, const FVector& Start, const FVector& End, const bool bHit,
	const FVector& ImpactPoint) const
{
	if (bDebug)
	{
		DrawDebugLine(World, Start, End, bHit ? FColor::Green : FColor::Red , true, 1.0f, 0, 2.0f);
		if (bHit)
		{
			DrawDebugPoint(GetWorld(), ImpactPoint, 20.0f, FColor::Cyan, true);
		}
	}
}
#endif
