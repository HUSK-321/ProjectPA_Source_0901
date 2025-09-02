// All copyrights for this code are owned by Aster.


#include "PAActorBase.h"
#include "Net/UnrealNetwork.h"
#include "ProjectPA/PALog.h"
#include "ProjectPA/Network/UIDContainer.h"

APAActorBase::APAActorBase()
	: ActorUID(INDEX_NONE)
{
	PrimaryActorTick.bCanEverTick = false;
}

void APAActorBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		RequestUID();
	}
}

void APAActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APAActorBase, ActorUID);
}

void APAActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APAActorBase::RequestUID()
{
	UUIDContainer::GetInstance(this)->RegisterObject(GetContainingType(), this);
}

EUIDType APAActorBase::GetContainingType() const
{
	return EUIDType::ItemContainer;
}

void APAActorBase::SetUID(int32 NewUID)
{
	ActorUID = NewUID;
	OnRep_UID();
}

void APAActorBase::OnRep_UID()
{
	IPAActor::OnRep_UID();
	PALog_W(TEXT("%s UID ? %d"), *GetName(), GetUID());
	UUIDContainer::GetInstance(this)->OnClientReplicatedUID(GetContainingType(), this);
}
