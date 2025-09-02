// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectPA/Characters/NPCs/PersonalityTraitSystem/PersonalityTraitTypes.h"
#include "PersonalityTraitInteractable.generated.h"

class AGameCharacterBase;

// This class does not need to be modified.
UINTERFACE()
class UPersonalityTraitInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 게임 내 인격적으로 상호작용이 가능한 캐릭터/오브젝트를 위한 인터페이스
 */
class PROJECTPA_API IPersonalityTraitInteractable
{
	GENERATED_BODY()

public:
	/** 상속받은 함수들의 경우 필히 FToPlayerRelationships와 FActionWeights를 가지고 아래 함수를 구현해야 합니다. */
	virtual FToPlayerRelationships& GetToPlayerRelationships() = 0;
	virtual int32 GetActionTypeWeight(EPersonalityActionType ActionType) { return INDEX_NONE; }
	
	virtual void ApplyAction(AGameCharacterBase* Causer, EPlayerAction ActionType) { }
};
