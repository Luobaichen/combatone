#include "Animation/CombatAnimNotifyState_Trace.h"

#include "Combat/CombatWeapon.h"
#include "Combat/MeleeTraceComponent.h"
#include "CombatCore.h"
#include "CombatCoreCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UCombatAnimNotifyState_Trace::NotifyBegin(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    float TotalDuration,
    const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (!MeshComp)
    {
        return;
    }

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        return;
    }

    UMeleeTraceComponent* Trace = Owner->FindComponentByClass<UMeleeTraceComponent>();

    ACombatCoreCharacter* Character = Cast<ACombatCoreCharacter>(Owner);

    if (!Trace || !Character)
    {
        UE_LOG(LogCombatCore, Warning,
            TEXT("[Notify] Trace begin ignored on %s: trace component or character missing"),
            *GetNameSafe(Owner));
        return;
    }

    ACombatWeapon* Weapon = Character->GetWeapon();
    if (!Weapon)
    {
        UE_LOG(LogCombatCore, Warning,
            TEXT("[Notify] Trace begin ignored on %s: weapon missing"), *GetNameSafe(Owner));
        return;
    }

    Trace->StartTrace(Weapon, Damage, Radius, bHeavyAttack);

    UE_LOG(LogCombatCore, Log,
        TEXT("[Notify] Begin %s: Damage=%.1f, Radius=%.1f, Heavy=%s"),
        *GetNameSafe(Animation), Damage, Radius, bHeavyAttack ? TEXT("yes") : TEXT("no"));
}

void UCombatAnimNotifyState_Trace::NotifyEnd(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (!MeshComp)
    {
        return;
    }

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        return;
    }

    if (UMeleeTraceComponent* Trace = Owner->FindComponentByClass<UMeleeTraceComponent>())
    {
        Trace->StopTrace();
        UE_LOG(LogCombatCore, Log, TEXT("[Notify] End %s"), *GetNameSafe(Animation));
    }
}