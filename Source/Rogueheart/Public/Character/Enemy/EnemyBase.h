#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class ROGUEHEART_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 추적
    void MoveToPlayer();

    // 공격 시도
    void TryAttack();

    // 감지 거리
    UPROPERTY(EditAnywhere, Category = "AI")
    float DetectRange = 800.f;

    // 공격 거리
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRange = 150.f;

    // 공격 쿨타임
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackCooldown = 2.f;

    // 애니메이션 몽타주
    UPROPERTY(EditAnywhere, Category = "AI")
    UAnimMontage* AttackMontage;

    // 타겟
    APawn* TargetPlayer;

private:
    float TimeSinceLastAttack = 0.f;
};