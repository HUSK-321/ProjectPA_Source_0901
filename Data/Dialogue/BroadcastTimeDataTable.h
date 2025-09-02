// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPA/Generals/PATypes.h"
#include "UObject/Object.h"
#include "BroadcastTimeDataTable.generated.h"

/**
 * 방송 시간을 나타내는 데이터 테이블
 */
USTRUCT(BlueprintType)
struct FBroadcastTimeDataTable : public FTableRowBase
{
    GENERATED_BODY()

    // 방송 시작 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
    FGameTimeInfo StartTime;
    // 방송 지속 시간 (인게임 분 단위)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
    int32 DurationMinutes = 10;
    // 방송 대사들 (여러 줄)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content", meta = (MultiLine = true))
    TArray<FName> DialogueKeys;

    /** Helper Functions */
    FGameTimeInfo GetEndTime() const
    {
        FGameTimeInfo Duration;
        Duration.Year = 0;
        Duration.Month = 0;
        Duration.Day = 0;
        Duration.Hour = DurationMinutes / 60;
        Duration.Min = DurationMinutes % 60;
        
        return StartTime + Duration;
    }

    bool IsTimeInBroadcastRange(const FGameTimeInfo& CurrentTime) const
    {
        return IsTimeInRange(CurrentTime, StartTime, GetEndTime());
    }

private:
    
    static bool IsTimeInRange(const FGameTimeInfo& CheckTime, const FGameTimeInfo& StartTime, const FGameTimeInfo& EndTime)
    {
        // 같은 날 내에서 방송이 끝나는 경우
        if (StartTime.Year == EndTime.Year && StartTime.Month == EndTime.Month && StartTime.Day == EndTime.Day)
        {
            return (CompareTime(CheckTime, StartTime) >= 0) && (CompareTime(CheckTime, EndTime) < 0);
        }
        // 자정을 넘어가는 방송의 경우
        else
        {
            return (CompareTime(CheckTime, StartTime) >= 0) || (CompareTime(CheckTime, EndTime) < 0);
        }
    }

    static int32 CompareTime(const FGameTimeInfo& A, const FGameTimeInfo& B)
    {
        if (A.Year != B.Year) return A.Year > B.Year ? 1 : -1;
        if (A.Month != B.Month) return A.Month > B.Month ? 1 : -1;
        if (A.Day != B.Day) return A.Day > B.Day ? 1 : -1;
        if (A.Hour != B.Hour) return A.Hour > B.Hour ? 1 : -1;
        if (A.Min != B.Min) return A.Min > B.Min ? 1 : -1;
        return 0;
    }
};

/**
 * 실제 방송 내용을 나타내는 데이터 테이블
 */
USTRUCT(BlueprintType)
struct FBroadcastDataTable : public FTableRowBase
{
    GENERATED_BODY()

public:
    /** 방송인 이름 */
    UPROPERTY(EditAnywhere)
    FText Broadcaster;
    /** 방송 텍스트 */
    UPROPERTY(EditAnywhere)
    FText BroadcastText;
};
