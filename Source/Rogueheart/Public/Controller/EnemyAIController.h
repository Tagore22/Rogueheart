#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

UCLASS()
class ROGUEHEART_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

    virtual void Tick(float DeltaSeconds) override;

    /** 팀 ID 반환 */
    virtual FGenericTeamId GetGenericTeamId() const override;

protected:
    virtual void BeginPlay() override;

    /** 감지 이벤트 처리 */
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
    /** AI Perception 컴포넌트 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UAIPerceptionComponent* AIPerception;

    /** 시야 설정 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UAISenseConfig_Sight* SightConfig;

    /** 공격 거리 */
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackDistance = 200.f;

    /** 현재 타겟 */
    UPROPERTY()
    AActor* TargetActor;

    /** 이 AI의 팀 ID */
    static constexpr uint8 EnemyTeamId = 2;
};