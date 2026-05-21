#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyBase.h"
#include "MeleeEnemy.generated.h"

UCLASS()
class ROGUEHEART_API AMeleeEnemy : public AEnemyBase
{
    GENERATED_BODY()

public:
    AMeleeEnemy();

    virtual void TryAttack() override;
protected:
    /** 원형 이동 반지름 */
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float CircleRadius = 250.f;

    /** 원형 이동 시 이동 거리 */
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float CircleSpeed = 200.f;

    /** 공격 간격 */
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float AttackInterval = 3.f;

    // 이후 이 클래스를 BP화하여 여러개를 만들어 적마다 다른 몽타주를 넣으면 된다.
    UPROPERTY(EditDefaultsOnly, Category = "AnimationMontage")
    class UAnimMontage* AMT_Attack;
};