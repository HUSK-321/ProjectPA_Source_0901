// All copyrights for this code are owned by Aster.


#include "WeaponItemStatWidget.h"
#include "Components/TextBlock.h"
#include "ProjectPA/Items/ItemDataTables.h"

void UWeaponItemStatWidget::SetItemStatus(const FItemDataTable* InItemData, const FItemEncodedInfo& InItemInfo)
{
	Super::SetItemStatus(InItemData, InItemInfo);

	if (DamageText != nullptr && RangeText != nullptr && AngleText != nullptr)
	{
		const FWeaponItemDataTable* WeaponData = static_cast<const FWeaponItemDataTable*>(InItemData);
		if (WeaponData)
		{
			DamageText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), WeaponData->AttackDamage)));
			RangeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), WeaponData->AttackRange)));
			AngleText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), WeaponData->AttackAngle)));
		}
	}
}
