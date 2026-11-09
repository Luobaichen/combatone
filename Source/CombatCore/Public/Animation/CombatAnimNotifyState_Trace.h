#pragma once

#include "CoreMinimal.h"
#include"Animation/AnimNotifies/AnimNotifyState.h"
#include"CombatAnimNotifyState_Trace.generated.h"


class UAnimSequenceBase;
class USkeletalMeshComponent;

UCLASS(meta=(DisplayName="Combat Trace Window"))
class COMBATCORE_API UCombatAnimNotifyState_Trace : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Combat|Trace",meta= (ClampMin="0.0"))
	float Damage = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Trace", meta = (ClampMin = "1.0"))
	float Radius = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Trace")
	bool bHeavyAttack = false;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,const FAnimNotifyEventReference& EventReference) override;

};