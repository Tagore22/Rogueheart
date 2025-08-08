#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
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

    /** 공격 시도 */
    void TryAttack();

    /** 타겟 마커 표시/숨기기 */
    void ShowTargetMarker(bool bShow);
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** AI Perception이 감지 상태를 업데이트할 때 호출 */
    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    /** 플레이어를 향해 이동 */
    void MoveToPlayer();

private:
    /** 현재 플레이어를 감지 중인지 여부 */
    bool bHasPerceivedPlayer = false;

    /** 마지막 공격 이후 경과 시간 */
    float TimeSinceLastAttack = 0.f;

public:
    /** 공격 범위 */
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRange = 150.f;

    /** 공격 쿨타임 */
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackCooldown = 2.f;

    /** 공격 모션 */
    UPROPERTY(EditAnywhere, Category = "AI")
    UAnimMontage* AttackMontage;

    /** 타겟 마커 UI */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* TargetMarker;

    /** 현재 추적 중인 플레이어 */
    UPROPERTY()
    APawn* TargetPlayer;

    /** 순찰 포인트들 */
    UPROPERTY(EditInstanceOnly, Category = "AI|Patrol")
    TArray<AActor*> PatrolPoints;

    /** 현재 순찰 인덱스 */
    UPROPERTY(VisibleAnywhere, Category = "AI|Patrol")
    int32 CurrentPatrolIndex = 0;
};