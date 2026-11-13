#include"Combat/CombatAttributeComponent.h"

UCombatAttributeComponent::UCombatAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetAttributes();
	
}

void UCombatAttributeComponent::ResetAttributes()
{
	Health = MaxHealth;
	Stamina = MaxStamina;
	bIsDead = false;
}

float UCombatAttributeComponent::ApplyDamage(float Amount, AActor* DamageCauser)
{
	if (Amount <= 0.f || bIsDead) {
		return 0.f;
	}

	const float ActualDamage = FMath::Min(Amount, Health);
	
	Health -= ActualDamage;

	OnHealthChanged.Broadcast(Health, MaxHealth);

	if (Health <= 0.f)
	{
		bIsDead = true;
		OnDeath.Broadcast(DamageCauser);
	}

	return ActualDamage;


}

float UCombatAttributeComponent::Heal(float Amount)
{
	if (Amount <= 0.f || bIsDead)
	{
		return 0.f;
	}

	const float ActualHeal = FMath::Min(Amount, MaxHealth - Health);
	Health += ActualHeal;

	if (ActualHeal > 0.f)
	{
		OnHealthChanged.Broadcast(Health, MaxHealth);
	}

	return ActualHeal;
}

bool UCombatAttributeComponent::TryConsumeStamina(float Amount)
{
	if (Amount <= 0.f || bIsDead)
	{
		return false;

	}

	if (Stamina < Amount)     
	{
		return false;
	}

	Stamina -= Amount;
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
	return true;
}

void UCombatAttributeComponent::RestoreStamina(float Amount)
{
	if (Amount <= 0.f || bIsDead)
	{
		return;
	}

	const float ActualRestore = FMath::Min(Amount, MaxStamina - Stamina);
	Stamina += ActualRestore;

	if (ActualRestore > 0.f)
	{
		OnStaminaChanged.Broadcast(Stamina, MaxStamina);
	}
}

float UCombatAttributeComponent::GetHealth() const
{
	return Health;
}

float UCombatAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UCombatAttributeComponent::GetStamina() const
{
	return Stamina;
}

float UCombatAttributeComponent::GetMaxStamina() const
{
	return MaxStamina;
}

bool UCombatAttributeComponent::IsDead() const
{
	return bIsDead;
}