#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatWeapon.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;

UCLASS()
class COMBATCORE_API ACombatWeapon : public AActor
{
	GENERATED_BODY()

public:
	ACombatWeapon();

    UFUNCTION(BlueprintPure, Category = "Combat|Weapon")
    FVector GetTraceStart() const;

    /** End of the blade, in world space. */
    UFUNCTION(BlueprintPure, Category = "Combat|Weapon")
    FVector GetTraceEnd() const;

    /** Attach this weapon to a socket on the character's skeletal mesh. */
    UFUNCTION(BlueprintCallable, Category = "Combat|Weapon")
    void AttachToCharacter(USkeletalMeshComponent* CharacterMesh, FName HandSocket);

    /** Optional socket names. Leave as None to use the offsets below. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
    FName TraceStartSocket = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
    FName TraceEndSocket = NAME_None;

    /** Blade start/end in the weapon actor's local space, in centimeters. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
    FVector TraceStartOffset = FVector(5.f, 0.f, 0.f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
    FVector TraceEndOffset = FVector(85.f, 0.f, 0.f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
    FRotator AttachRotationOffset = FRotator(90.f, 0.f, 0.f);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	TObjectPtr<UStaticMeshComponent> Mesh;
};
