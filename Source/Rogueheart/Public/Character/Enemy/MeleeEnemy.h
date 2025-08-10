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
    /** ���� �̵� ������ */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float CircleRadius = 250.f;

    /** ���� �̵� �� �̵� �Ÿ� */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float CircleSpeed = 200.f;

    /** ���� ���� */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackInterval = 3.f;
};