#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;

UCLASS()
class ROGUEHEART_API UPlayerAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPlayerAnimInstance();

    //void SetIsAttacking(bool bNewState);

protected:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    virtual void NativeInitializeAnimation() override;

    UFUNCTION()
    void AnimNotify_EndAttack();

    UFUNCTION()
    void AnimNotify_NextCombo();

    UFUNCTION()
    void AnimNotify_EndDodge();

private:
    void ResetPlayerToIdle();

    // 이동 관련
    UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float Speed;

    //UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    //bool bIsInAir;

    //UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    //bool bIsAccelerating;


    // 전투 상태
    //UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    //bool bIsAttacking;


    // 락온 이동용 BlendSpace 파라미터
    UPROPERTY(BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
    float WalkSpeed;

    UPROPERTY(BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
    float Direct;

    UPROPERTY(BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess = "true"))
    bool bIsLockedOn;

    UPROPERTY()
    APlayerCharacter* Player = nullptr;
};