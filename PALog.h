// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(PALog, Log, All);

#define LOG_CALL_INFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PALog(Format, ...) UE_LOG(PALog, Log, TEXT("[%s] %s"), *LOG_CALL_INFO, *FString::Printf(Format, ##__VA_ARGS__))
#define PALog_W(Format, ...) UE_LOG(PALog, Warning, TEXT("[%s] %s"), *LOG_CALL_INFO, *FString::Printf(Format, ##__VA_ARGS__))
#define PALog_E(Format, ...) UE_LOG(PALog, Error, TEXT("[%s] %s"), *LOG_CALL_INFO, *FString::Printf(Format, ##__VA_ARGS__))