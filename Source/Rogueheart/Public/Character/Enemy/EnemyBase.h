#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UWidgetComponent;

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

    // 플레이어 추적
    void MoveToPlayer();

    // 공격 시도
    void TryAttack();

    // UI 마커 제어용 함수
    void ShowTargetMarker(bool bShow);

public:
    // 감지 거리
    UPROPERTY(EditAnywhere, Category = "AI")
        float DetectRange = 800.f;

    // 공격 거리
    UPROPERTY(EditAnywhere, Category = "AI")
        float AttackRange = 150.f;

    // 공격 쿨타임
    UPROPERTY(EditAnywhere, Category = "AI")
        float AttackCooldown = 2.f;

    // 공격 애니메이션
    UPROPERTY(EditAnywhere, Category = "AI")
        UAnimMontage* AttackMontage;

    // 타겟 마커 위젯
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
        UWidgetComponent* TargetMarker;

    // 타겟 플레이어
    UPROPERTY()
        APawn* TargetPlayer;

private:
    float TimeSinceLastAttack = 0.f;
};