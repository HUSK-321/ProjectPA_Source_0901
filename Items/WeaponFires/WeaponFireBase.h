#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProjectPA/Characters/GameCharacterBase.h"
#include "WeaponFireBase.generated.h"

class AGameCharacterBase;

UENUM(BlueprintType)
enum class EWeaponFireType : uint8
{
    None,
    KnifeHorizontalSweep     UMETA(DisplayName = "Knife Horizontal Sweep"),
};

namespace WeaponFire
{
    namespace Traces
    {
        static void Execute_KnifeHorizontalSweep(const AGameCharacterBase* WeaponOwner, float FireRange, float FireAngle, TArray<FHitResult>& OutHitResults)
        {
            if (WeaponOwner == nullptr)
            {
                return;
            }
            
            const UWorld* World = WeaponOwner->GetWorld();
            if (World == nullptr)
            {
                return;
            }

            const FVector StartLocation = WeaponOwner->GetActorLocation();
            const FVector ForwardVector = WeaponOwner->GetActorForwardVector();
            const FVector EndLocation = StartLocation + (ForwardVector * FireRange);

            const float SphereRadius = FireRange * FMath::Tan(FMath::DegreesToRadians(FireAngle * 0.5f));

            FCollisionQueryParams TraceParams(FName(TEXT("WeaponSweepTrace")), true, WeaponOwner);
            World->SweepMultiByChannel(OutHitResults, StartLocation, EndLocation, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius), TraceParams);
        }
    }

    static void Execute(EWeaponFireType FireType, AGameCharacterBase* WeaponOwner, float FireRange, float FireAngle, TArray<FHitResult>& OutHitResults)
    {
        switch (FireType)
        {
        case EWeaponFireType::KnifeHorizontalSweep:
            Traces::Execute_KnifeHorizontalSweep(WeaponOwner, FireRange, FireAngle, OutHitResults);
            break;

        case EWeaponFireType::None:
        default:
            break;
        }
    }
}
