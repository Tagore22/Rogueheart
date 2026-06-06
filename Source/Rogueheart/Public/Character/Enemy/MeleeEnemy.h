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

    /*UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float CircleRadius = 250.f;


    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float CircleSpeed = 200.f;


    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float AttackInterval = 3.f;*/
};