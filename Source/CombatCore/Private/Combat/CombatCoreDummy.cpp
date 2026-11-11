#include"Combat/CombatCoreDummy.h"

#include "Animation/AnimMontage.h"
#include"Animation/AnimInstance.h"
#include"Combat/CombatAttributeComponent.h"
#include"Components/CapsuleComponent.h"
#include"Components/SkeletalMeshComponent.h"
#include"Components/TextRenderComponent.h"
#include"Engine/SkeletalMesh.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"UObject/ConstructorHelpers.h"

ACombatCoreDummy::ACombatCoreDummy()
{
	AttributeComponent = CreateDefaultSubobject<UCombatAttributeComponent>(TEXT("AttributeComponent"));
	
	HealthText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthText"));

	HealthText->SetupAttachment(GetCapsuleComponent());
	HealthText->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HealthText->SetHorizontalAlignment(EHTA_Center);
	HealthText->SetVerticalAlignment(EVRTA_TextCenter);
	HealthText->SetWorldSize(40.f);
	HealthText->SetTextRenderColor(FColor::Red);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	}

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FClassFinder<UAnimInstance>AnimBP(TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));

	if (AnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitReactAsset(
		TEXT("/Game/Animations/AM_HitReact.AM_HitReact"));
	if (HitReactAsset.Succeeded())
	{
		HitReactMontage = HitReactAsset.Object;
	}

}

void ACombatCoreDummy::BeginPlay()
{
	Super::BeginPlay();

	if (AttributeComponent) {

		AttributeComponent->OnHealthChanged.AddDynamic(this, &ACombatCoreDummy::HandleHealthChanged);
		AttributeComponent->OnDeath.AddDynamic(this, &ACombatCoreDummy::HandleDeath);

		UpdateHealthText(AttributeComponent->GetHealth(), AttributeComponent->GetMaxHealth());

	}
}

void ACombatCoreDummy::ApplyHit_Implementation(const FCombatHitInfo& HitInfo)
{
	if (!AttributeComponent || AttributeComponent->IsDead())
	{
		return;
	}

	const float ActualDamage = AttributeComponent->ApplyDamage(HitInfo.Damage, HitInfo.Instigator);

	UE_LOG(LogTemp, Log, TEXT("[Dummy]Hit by %s: Actual=%.1f,HP=%.1f/%.1f,Bone=%s,Heavy=%s"), *GetNameSafe(HitInfo.Instigator), ActualDamage, AttributeComponent->GetHealth(), AttributeComponent->GetMaxHealth(), *HitInfo.HitBoneName.ToString(), HitInfo.bHeavyAttack ? TEXT("yes") : TEXT("no"));

	if (!AttributeComponent->IsDead() && HitReactMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
		{
			if (!AnimInstance->IsAnyMontagePlaying())
			{
				PlayAnimMontage(HitReactMontage);
			}
		}
	}
}
void ACombatCoreDummy::HandleDeath(AActor* Killer)
{
	UE_LOG(LogTemp,Warning,TEXT("[Dummy] %s is dead.Killer: %s"), *GetName(), *GetNameSafe(Killer));

	if (UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		AnimInstance->StopAllMontages(0.1f);
	}

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	GetMesh()->SetSimulatePhysics(true);

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(TEXT("DEAD")));

	}

	
}

void ACombatCoreDummy::UpdateHealthText(float NewHealth, float MaxHealth)
{
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), NewHealth, MaxHealth)));
	}
}

void ACombatCoreDummy::HandleHealthChanged(float NewHealth, float MaxHealth)
{
	UpdateHealthText(NewHealth, MaxHealth);
}