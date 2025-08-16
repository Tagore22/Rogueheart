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

    /** 팀 ID 반환 */
    virtual FGenericTeamId GetGenericTeamId() const override;

    // ADDED: 매 Tick마다 디버깅용 시야각 그려줌
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

    /** 감지 이벤트 처리 */
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    /** AI Perception 컴포넌트 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class UAIPerceptionComponent* AIPerception;

    /** 시야 설정 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class UAISenseConfig_Sight* SightConfig;

    /** 비헤이비어 트리 에셋 */
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBehaviorTree* BehaviorTreeAsset;

    /** 블랙보드 에셋 */
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBlackboardData* BlackboardAsset;

    // ADDED: 조사(Investigate) 관련 설정(에디터에서 조절 가능)
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float InvestigateTimeout = 3.0f;

private:
    /** 이 AI의 팀 ID */
    static constexpr uint8 EnemyTeamId = 2;

    // ADDED: 조사 타이머 핸들
    FTimerHandle InvestigateTimerHandle;

    // ADDED: 조사 종료 처리 함수
    void StopInvestigating();

    // ADDED: 시야각 디버그 함수
    void Debug_DrawFOV();
};