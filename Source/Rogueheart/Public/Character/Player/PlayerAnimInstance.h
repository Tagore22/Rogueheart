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

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    void SetIsAttacking(bool bNewState) { bIsAttacking = bNewState; }

    UFUNCTION()
    void AnimNotify_EndAttack();

    UFUNCTION()
    void AnimNotify_EndDodge();

protected:
    // 이동 관련 상태
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsInAir;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsAccelerating;

    // 전투 상태
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bIsAttacking;

private:
    // 상태 복귀
    void ResetPlayerToIdle();
};