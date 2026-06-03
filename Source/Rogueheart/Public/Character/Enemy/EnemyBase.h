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

    /** 공격 시도 (비헤이비어 트리에서 호출) */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void TryAttack();

    /** 타겟 마커 표시/숨기기 */
    void ShowTargetWidget(bool bShow);

    void ShowHPBarWidget(bool bShow);

    void ResetHPBarTimer();

    void SetIsTargeted(bool bTargeted);

    /** 공격 쿨다운 체크 */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool CanAttack() const;

    /** 마지막 공격 시간 리셋 */
    void ResetAttackCooldown();

    UFUNCTION(BlueprintCallable)
    float GetCurHP() const;
    UFUNCTION(BlueprintCallable)
    float GetMaxHP() const;

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EnemyDie();

protected:
    bool bIsTargeted = false;

    /** 마지막 공격 이후 경과 시간 */
    float TimeSinceLastAttack = 0.f;

    float HPBarTimer = -1.f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TArray<class UAnimMontage*> AttackMontages;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TArray<class UAnimMontage*> DamagedMontages;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    class UAnimMontage* AMT_Die;


    UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true), Category = "HP")
    float MaxHP = 100.f;

    UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true), Category = "HP")
    float CurHP = 100.f;

    UPROPERTY(EditDefaultsOnly, Category = "DyingTime")
    float DyingTime = 3.f;

    UPROPERTY(EditDefaultsOnly, Category = "HPBarEndTime")
    float HPBarEndTime = 3.f;

    UPROPERTY(VisibleAnywhere, Category = "UI")
    class UWidgetComponent* HPBarWidget = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "UI")
    class UWidgetComponent* TargetWidget = nullptr;

public:
    /** 공격 범위 */
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float AttackRange = 150.f;

    /** 공격 쿨타임 */
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float AttackCooldown = 2.f;

    /** 순찰 포인트들 */
    UPROPERTY(EditInstanceOnly, Category = "AI|Patrol")
    TArray<AActor*> PatrolPoints;

    /** 현재 순찰 인덱스 */
    UPROPERTY(VisibleAnywhere, Category = "AI|Patrol")
    int32 CurrentPatrolIndex = 0;
};