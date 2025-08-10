#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UWidgetComponent;
class UAnimMontage;
class AActor;

UCLASS()
class ROGUEHEART_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyBase();

    /** 공격 시도 (비헤이비어 트리에서 호출) */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void TryAttack();

    /** 타겟 마커 표시/숨기기 */
    void ShowTargetMarker(bool bShow);

    /** 공격 쿨다운 체크 */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool CanAttack() const;

    /** 마지막 공격 시간 리셋 */
    void ResetAttackCooldown();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    /** 마지막 공격 이후 경과 시간 */
    float TimeSinceLastAttack = 0.f;

public:
    /** 공격 범위 */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackRange = 150.f;

    /** 공격 쿨타임 */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackCooldown = 2.f;

    /** 공격 모션 */
    UPROPERTY(EditAnywhere, Category = "Combat")
    UAnimMontage* AttackMontage;

    /** 타겟 마커 UI */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* TargetMarker;

    /** 순찰 포인트들 */
    UPROPERTY(EditInstanceOnly, Category = "AI|Patrol")
    TArray<AActor*> PatrolPoints;

    /** 현재 순찰 인덱스 */
    UPROPERTY(VisibleAnywhere, Category = "AI|Patrol")
    int32 CurrentPatrolIndex = 0;
};