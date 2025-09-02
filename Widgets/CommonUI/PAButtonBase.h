// All copyrights for this code are owned by Aster.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "ProjectPA/WIdgets/PAWidgets.h"
#include "PAButtonBase.generated.h"

/**
 * CommonButtonBase를 상속받는 Project PA의 기본 버튼 클래스입니다.
 */
UCLASS(Abstract, Blueprintable)
class PROJECTPA_API UPAButtonBase : public UCommonButtonBase, public IPAWidgets
{
	GENERATED_BODY()

public:
	UPAButtonBase();
};