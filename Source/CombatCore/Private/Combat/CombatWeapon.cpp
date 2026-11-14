#include "Combat/CombatWeapon.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ACombatWeapon::ACombatWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Mesh->SetRelativeLocation(FVector(45.f, 0.f, 0.f));
	Mesh->SetRelativeScale3D(FVector(0.9f, 0.06f, 0.06f));

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (CubeAsset.Succeeded())
	{
		Mesh->SetStaticMesh(CubeAsset.Object);
	}
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
