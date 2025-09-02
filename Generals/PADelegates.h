// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
//#include "PADelegates.generated.h"

enum class EBodyPart : uint8;

DECLARE_MULTICAST_DELEGATE_OneParam(FAttributeChanged, const FGameplayAttribute& /** ChangedAttribute */);
DECLARE_MULTICAST_DELEGATE_OneParam(FMovedDistanceChanged, const int32 /** MovedMeter */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FStatusChangedEvent, const float /** MaxValue */, const float /** CurValue */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FAttributeChangedEvent, const float /** OldValue */, const float /** CurValue */);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FBodyPartChanged, const EBodyPart /** InBodyPart */, const float /** InCurrentValue */, const float /** InMaxValue = 100.f */);
