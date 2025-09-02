// All copyrights for this code are owned by Aster.


#include "ConnectionManagerBase.h"

UConnectionManagerBase::UConnectionManagerBase()
	: ConnectionManagerActor(nullptr)
{
}

void UConnectionManagerBase::InitializeManager(AConnectionManager* InConnectionManagerActor)
{
	ConnectionManagerActor = InConnectionManagerActor;
}
