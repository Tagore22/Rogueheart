#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyBase.h"
#include "MeleeEnemy.generated.h"

UENUM(BlueprintType)
enum class EMeleeEnemyState : uint8
{
    Idle,
    Approaching,
    Circling,
    Attacking,
    Cooldown
};

UCLASS()
class ROGUEHEART_API AMeleeEnemy : public AEnemyBase
{
    GENERATED_BODY()

public:
    AMeleeEnemy();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    void UpdateState(float DeltaTime);
    void TryCirclePlayer();
    void PerformMeleeAttack();
    void MoveAroundPlayer();
    void SetEnemyState(EMeleeEnemyState NewState);

    bool ShouldAttack() const;

public:
    UPROPERTY(EditAnywhere, Category = "AI")
    float CircleRadius = 250.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float CircleSpeed = 200.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackInterval = 3.f;

private:
    EMeleeEnemyState CurrentState = EMeleeEnemyState::Idle;

    float TimeSinceLastAction = 0.f;
};