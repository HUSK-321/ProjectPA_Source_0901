#include "GameCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Net/UnrealNetwork.h"
#include "MotionWarpingComponent.h"

#include "ProjectPA/PALog.h"
#include "ProjectPA/Components/CharacterAbilitySystemComponent.h"
#include "ProjectPA/Components/EquipmentComponent.h"
#include "ProjectPA/Interfaces/SwimmableVolume.h"
#include "ProjectPA/Network/UIDContainer.h"

AGameCharacterBase::AGameCharacterBase()
	: ActorSwimLocationOffsetZ(-44.0f)
	, ActorUID(INDEX_NONE)
	, bCharacterDeadStart(false)
	, bCharacterDeadEnd(false)
	, bIsSwimming(false)
{
	PrimaryActorTick.bCanEverTick = false;
	
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	EquipmentComponent->SetupAttachment(GetMesh(), TEXT("righthand_equip"));
}

void AGameCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameCharacterBase, bCharacterDeadStart);
	DOREPLIFETIME(AGameCharacterBase, bCharacterDeadEnd);
}

void AGameCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	SetOwner(Controller);
}

UAbilitySystemComponent* AGameCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AGameCharacterBase::CanPlayFootstepSound() const
{
	if (GetCharacterMovement())
	{
		EMovementMode MovementMode = GetCharacterMovement()->MovementMode;
		return MovementMode != MOVE_Flying && MovementMode != MOVE_Swimming;
	}
	return false;
}

FTransform AGameCharacterBase::GetRootBoneTransform() const
{
	if (IsValid(GetMesh()))
	{
		const FTransform RootTransform = GetMesh()->GetBoneTransform(TEXT("root"));
		FRotator RootRotator = RootTransform.GetRotation().Rotator();
		RootRotator.Yaw += 90.0f;

		return FTransform{RootRotator, RootTransform.GetLocation()};
	}
	return FTransform();
}

void AGameCharacterBase::StopAllActions()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	StopAnimMontage();
}

void AGameCharacterBase::StartDying()
{
	bCharacterDeadStart = true;
	StopAllActions();

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void AGameCharacterBase::DyingEnd()
{
	bCharacterDeadEnd = true;
}

void AGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		RequestUID();
	}
	GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::PostBeginPlay);
}

void AGameCharacterBase::PostBeginPlay()
{
	OnAbilitySystemReady();
}

void AGameCharacterBase::StartSwimming(TScriptInterface<ISwimmableVolume> SwimmableVolume)
{
	SetSwimming(true);

	FVector CurrentVelocity = GetMovementComponent()->Velocity;
	CurrentVelocity.Z = 0.0f;
	GetMovementComponent()->Velocity = CurrentVelocity;
	
	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z = SwimmableVolume->GetSwimmableLocationZ() + ActorSwimLocationOffsetZ;
	SetActorLocation(ActorLocation);
}

void AGameCharacterBase::StopSwimming(TScriptInterface<ISwimmableVolume> SwimmableVolume)
{
	SetSwimming(false);
}

void AGameCharacterBase::SetSwimming(bool bSwimming)
{
	if (bIsSwimming == bSwimming)
	{
		return;
	}
	
	bIsSwimming = bSwimming;
	
	if (UCharacterMovementComponent* OwnerCharacterMovement = GetCharacterMovement())
	{
		if (APhysicsVolume* PhysicsVolume = OwnerCharacterMovement->GetPhysicsVolume())
		{
			PhysicsVolume->bWaterVolume = bIsSwimming;
		}
		OwnerCharacterMovement->SetMovementMode(bSwimming ? MOVE_Swimming : MOVE_Walking);
	}
}

void AGameCharacterBase::UnequipAndTransferAllItemsOnDeath()
{
	if (!HasAuthority() || !IsValid(EquipmentComponent))
	{
		return;
	}

	// Get EquipmentComponent's items to lootable place
	TArray<FItemEncodedInfo> AllEquippedItems;
	EquipmentComponent->GetAllEquippedAndWornItems(AllEquippedItems);
	AddItemsToLootOnDeath(AllEquippedItems);

	EquipmentComponent->ClearAllEquipment();
}

void AGameCharacterBase::RequestUID()
{
	UUIDContainer::GetInstance(this)->RegisterObject(GetContainingType(), this);
}

EUIDType AGameCharacterBase::GetContainingType() const
{
	return EUIDType::Character;
}

void AGameCharacterBase::OnRep_UID()
{
	IPAActor::OnRep_UID();
	UUIDContainer::GetInstance(this)->OnClientReplicatedUID(GetContainingType(), this);
}
