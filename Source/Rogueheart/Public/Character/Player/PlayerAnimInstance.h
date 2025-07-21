#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class ROGUEHEART_API UPlayerAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPlayerAnimInstance();

    // 매 프레임 호출
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    void SetIsAttacking(bool bNewState) { bIsAttacking = bNewState; }

    UFUNCTION()
    void AnimNotify_EndAttack();
    UFUNCTION()
    void AnimNotify_EndDodge();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsInAir;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsAccelerating;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bIsAttacking;

private:
    // 공통 helper
    void ResetPlayerToIdle();
};