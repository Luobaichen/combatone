#include "Combat/CombatWeapon.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

ACombatWeapon::ACombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// No visible mesh by default: the fists are the weapon. This actor only
	// carries the trace points, attached to the hand socket in BeginPlay.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);
}

FVector ACombatWeapon::GetTraceStart() const
{
	if (Mesh && TraceStartSocket != NAME_None && Mesh->DoesSocketExist(TraceStartSocket))
	{
		return Mesh->GetSocketLocation(TraceStartSocket);
	}

	return GetActorTransform().TransformPosition(TraceStartOffset);
}

FVector ACombatWeapon::GetTraceEnd() const
{
	if (Mesh && TraceEndSocket != NAME_None && Mesh->DoesSocketExist(TraceEndSocket))
	{
		return Mesh->GetSocketLocation(TraceEndSocket);
	}

	return GetActorTransform().TransformPosition(TraceEndOffset);
}

void ACombatWeapon::AttachToCharacter(USkeletalMeshComponent* CharacterMesh, FName HandSocket)
{
	if (!CharacterMesh)
	{
		return;
	}

	AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocket);

	AddActorLocalRotation(AttachRotationOffset);
}
