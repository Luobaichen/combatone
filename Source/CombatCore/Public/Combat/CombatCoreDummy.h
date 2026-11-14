#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/CombatDamageable.h"
#include "CombatCoreDummy.generated.h"

class UCombatAttributeComponent;
class UTextRenderComponent;
class UAnimMontage;

/** A training dummy. Implements the same damage interface as the player. */
UCLASS()
class COMBATCORE_API ACombatCoreDummy : public ACharacter, public ICombatDamageable
{
	GENERATED_BODY()

public:
	ACombatCoreDummy();

	virtual void ApplyHit_Implementation(const FCombatHitInfo& HitInfo) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleHealthChanged(float NewHealth, float MaxHealth);

	UFUNCTION()
	void HandleDeath(AActor* Killer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UCombatAttributeComponent> AttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UTextRenderComponent> HealthText;

private:
	void UpdateHealthText(float NewHealth, float MaxHealth);

};
