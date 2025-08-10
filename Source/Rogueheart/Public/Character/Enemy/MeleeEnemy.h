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

public:
    /** 원형 이동 반지름 */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float CircleRadius = 250.f;

    /** 원형 이동 시 이동 거리 */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float CircleSpeed = 200.f;

    /** 공격 간격 */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackInterval = 3.f;
};