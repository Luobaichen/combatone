#pragma once

#include"CoreMinimal.h"
#include"Components/ActorComponent.h"
#include"CombatAttributeComponent.generated.h"

class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, NewStamina, float, MaxStamina);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath,AActor*,Killer);

UCLASS(ClassGroup = (Combat), meta = (BlueprintSpawnableComponent))
class COMBATCORE_API UCombatAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatAttributeComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Attributes", meta = (ClampMin ="1.0"))

	float MaxHealth = 100.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Attributes", meta = (ClampMin = "1.0"))
	float MaxStamina = 100.f;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnStaminaChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable, Category = "Combat|Attributes")
	float ApplyDamage(float Amount, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Combat|Attributes")
	float Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Combat|Attributes")
	bool TryConsumeStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Combat|Attributes")
	void RestoreStamina(float Amount);

	UFUNCTION(BlueprintPure, Category = "Combat|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Attributes")
	bool IsDead() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Attributes", meta = (AllowPrivateAccess = "true"))
	float Health = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Attributes", meta = (AllowPrivateAccess = "true"))
	float Stamina = 0.f;

	bool bIsDead = false;


};