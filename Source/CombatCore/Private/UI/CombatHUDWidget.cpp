#include "UI/CombatHUDWidget.h"

#include "Combat/CombatAttributeComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UCombatHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    Refresh();
}

void UCombatHUDWidget::NativeDestruct()
{
    SetAttributeComponent(nullptr);
    Super::NativeDestruct();
}

void UCombatHUDWidget::HandleHealthChanged(float NewHealth, float MaxHealth)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(MaxHealth > 0.f ? NewHealth / MaxHealth : 0.f);
    }

    if (HealthText)
    {
        HealthText->SetText(FText::FromString(
            FString::Printf(TEXT("%.0f / %.0f"), NewHealth, MaxHealth)));
    }
}

void UCombatHUDWidget::HandleStaminaChanged(float NewStamina, float MaxStamina)
{
    if (StaminaBar)
    {
        StaminaBar->SetPercent(MaxStamina > 0.f ? NewStamina / MaxStamina : 0.f);
    }

    if (StaminaText)
    {
        StaminaText->SetText(FText::FromString(
            FString::Printf(TEXT("%.0f / %.0f"), NewStamina, MaxStamina)));
    }
}

void UCombatHUDWidget::SetAttributeComponent(UCombatAttributeComponent* InComponent)
{
    if (AttributeComponent)
    {
        AttributeComponent->OnHealthChanged.RemoveDynamic(this, &UCombatHUDWidget::HandleHealthChanged);
        AttributeComponent->OnStaminaChanged.RemoveDynamic(this, &UCombatHUDWidget::HandleStaminaChanged);
    }

    AttributeComponent = InComponent;

    if (AttributeComponent)
    {
        AttributeComponent->OnHealthChanged.AddDynamic(this, &UCombatHUDWidget::HandleHealthChanged);
        AttributeComponent->OnStaminaChanged.AddDynamic(this, &UCombatHUDWidget::HandleStaminaChanged);
    }

    Refresh();
}

void UCombatHUDWidget::Refresh()
{
    const float Health = AttributeComponent ? AttributeComponent->GetHealth() : 0.f;
    const float MaxHealth = AttributeComponent ? AttributeComponent->GetMaxHealth() : 1.f;
    const float Stamina = AttributeComponent ? AttributeComponent->GetStamina() : 0.f;
    const float MaxStamina = AttributeComponent ? AttributeComponent->GetMaxStamina() : 1.f;

    HandleHealthChanged(Health, MaxHealth);
    HandleStaminaChanged(Stamina, MaxStamina);
}

