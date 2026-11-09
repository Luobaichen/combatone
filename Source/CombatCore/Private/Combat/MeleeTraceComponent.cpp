// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/MeleeTraceComponent.h"

#include "Combat/CombatDamageable.h"
#include "Combat/CombatTypes.h"
#include "Combat/CombatWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UMeleeTraceComponent::UMeleeTraceComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	
}

void UMeleeTraceComponent::StartTrace(ACombatWeapon* InWeapon, float InDamage, float InRadius, bool bInHeavyAttack )
{
	if (!InWeapon) {
		return;
	}

	Weapon = InWeapon;
	Damage = InDamage;
	Radius = FMath::Max(InRadius, 1.f);
	bHeavyAttack = bInHeavyAttack;

	HitActors.Reset();

	PreviousStart = Weapon->GetTraceStart();
	PreviousEnd = Weapon->GetTraceEnd();

	bTracing = true;
	SetComponentTickEnabled(true);

	UE_LOG(LogTemp, Log, TEXT("[Trace] Start (Damage=%.1f, Radius=%.1f)"), Damage, Radius);
}

void UMeleeTraceComponent::StopTrace()
{
	bTracing = false;
	SetComponentTickEnabled(false);
	Weapon = nullptr;

	UE_LOG(LogTemp, Log, TEXT("[Trace] Stop"));
}

void UMeleeTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bTracing || !IsValid(Weapon)) {
		StopTrace();
		return;
	}

	PerformSweep();

	PreviousStart = Weapon->GetTraceStart();
	PreviousEnd = Weapon->GetTraceEnd();
}

void UMeleeTraceComponent::PerformSweep()
{

	UWorld* World = GetWorld();
	if (!World || !Weapon) {
		return;
	}

	const FVector CurrentStart = Weapon->GetTraceStart();
	const FVector CurrentEnd = Weapon->GetTraceEnd();

	FCollisionQueryParams Params(NAME_None, false, GetOwner());
	Params.AddIgnoredActor(Weapon);

	const FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);
	TArray<FHitResult> Hits;

	World->SweepMultiByChannel(Hits, PreviousStart, CurrentStart, FQuat::Identity, ECC_Pawn, Shape, Params);

	World->SweepMultiByChannel(Hits, PreviousEnd, CurrentEnd, FQuat::Identity, ECC_Pawn, Shape, Params);

	World->SweepMultiByChannel(Hits, CurrentStart, CurrentEnd, FQuat::Identity, ECC_Pawn, Shape, Params);


	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) {
			continue;
		}

		const TWeakObjectPtr<AActor> WeakHit(HitActor);
		if (HitActors.Contains(WeakHit)) {
			continue;
		}
		HitActors.Add(WeakHit);

		if (!HitActor->Implements<UCombatDamageable>())
		{
			continue;
		}


		FCombatHitInfo HitInfo;

		HitInfo.Damage = Damage;
		HitInfo.HitLocation = Hit.ImpactPoint;
		HitInfo.HitNormal = Hit.ImpactNormal;
		HitInfo.Instigator = GetOwner();
		HitInfo.HitBoneName = Hit.BoneName;
		HitInfo.bHeavyAttack = bHeavyAttack;

		ICombatDamageable::Execute_ApplyHit(HitActor, HitInfo);

		
		UE_LOG(LogTemp, Log, TEXT("[Trace] Hit %s (Bone=%s)"), *GetNameSafe(HitActor), *Hit.BoneName.ToString());


	}

	if (bDrawDebug)
	{
		DrawDebugSphere(World, CurrentStart, Radius, 8, FColor::Green, false, 0.5f);
		DrawDebugSphere(World, CurrentEnd, Radius, 8, FColor::Red, false, 0.5f);
		DrawDebugLine(World, CurrentStart, CurrentEnd, FColor::Yellow, false, 0.5f, 0, 2.f);
		DrawDebugLine(World, PreviousStart, CurrentStart, FColor::Cyan, false, 0.5f, 0, 1.f);
		DrawDebugLine(World, PreviousEnd, CurrentEnd, FColor::Cyan, false, 0.5f, 0, 1.f);
	}
}













