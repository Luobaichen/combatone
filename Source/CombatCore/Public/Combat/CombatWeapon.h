#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatWeapon.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;

/** Invisible trace anchor. Fists are the weapon: attach it to a hand socket and
 *  both trace points sit on the hand. Add a mesh here for a real weapon. */
UCLASS()
class COMBATCORE_API ACombatWeapon : public AActor
{
	GENERATED_BODY()

public:
	ACombatWeapon();

    UFUNCTION(BlueprintPure, Category = "Combat|Weapon")
    FVector GetTraceStart() const;

    /** End of the trace segment, in world space. */
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

    /** Trace start/end in the anchor's local space, in centimeters.
     *  Both are zero so the sweep spheres follow the fist. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
    FVector TraceStartOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
    FVector TraceEndOffset = FVector::ZeroVector;

    /** Extra rotation after snapping to the hand socket. Zero for fists. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
    FRotator AttachRotationOffset = FRotator::ZeroRotator;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	TObjectPtr<UStaticMeshComponent> Mesh;
};
