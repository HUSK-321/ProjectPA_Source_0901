// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Interfaces/PAActor.h"
#include "UObject/Object.h"
#include "UIDContainer.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EUIDType : uint8
{
	Character,
	ItemContainer,
};

UCLASS()
class PROJECTPA_API UUIDContainer : public UObject
{
	GENERATED_BODY()

private:
	static TObjectPtr<UUIDContainer> Instance;
	
	UPROPERTY()
	TMap<int32, TScriptInterface<IPAActor>> ActorContainer;
	UPROPERTY()
	TMap<int32, TScriptInterface<IPAActor>> CharacterContainer;
	UPROPERTY()
	TMap<int32, TScriptInterface<IPAActor>> ItemContainer;

	int32 AvailableUID = 0;

public:
	static UUIDContainer* GetInstance(UObject* Context);
	
	void RegisterObject(EUIDType UIDType, IPAActor* InActor);
	void OnClientReplicatedUID(EUIDType UIDType, AActor* InActor);

	TScriptInterface<IPAActor> GetObjectByUID(int32 UID);
	TScriptInterface<IPAActor> GetObjectByUID(EUIDType UIDType, int32 UID);
	TScriptInterface<IPAActor> GetObjectByUID(uint8 UIDType, int32 UID);

private:
	int32 GetAvailableUID();
	TMap<int32, TScriptInterface<IPAActor>>& GetContainer(EUIDType UIDType);
};
