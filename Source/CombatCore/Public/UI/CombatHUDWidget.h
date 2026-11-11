#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatHUDWidget.generated.h"

class UCombatAttributeComponent;
class UProgressBar;
class UTextBlock;

UCLASS(Blueprintable)
class COMBATCORE_API UCombatHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Combat|HUD")
    void SetAttributeComponent(UCombatAttributeComponent* InComponent);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void HandleHealthChanged(float NewHealth, float MaxHealth);

    UFUNCTION()
    void HandleStaminaChanged(float NewStamina, float MaxStamina);

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HealthBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> StaminaBar;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> HealthText;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> StaminaText;


private:
    void Refresh();

    UPROPERTY()
    TObjectPtr<UCombatAttributeComponent> AttributeComponent;


};