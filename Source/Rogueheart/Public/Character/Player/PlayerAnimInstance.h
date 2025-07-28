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

    // ���� �� ���� ����
    void SetIsAttacking(bool bNewState) { bIsAttacking = bNewState; }

protected:
    // �ִϸ��̼� �˸�
    UFUNCTION()
        void AnimNotify_EndAttack();

    UFUNCTION()
        void AnimNotify_NextCombo();

    UFUNCTION()
        void AnimNotify_EndDodge();

private:
    void ResetPlayerToIdle();

    // �̵� ����
    UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
        float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
        bool bIsInAir;

    UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
        bool bIsAccelerating;

    // ���� ����
    UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
        bool bIsAttacking;
};