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

    /** �� ID ��ȯ */
    virtual FGenericTeamId GetGenericTeamId() const override;

    // ADDED: �� Tick���� ������ �þ߰� �׷���
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

    /** ���� �̺�Ʈ ó�� */
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    /** AI Perception ������Ʈ */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class UAIPerceptionComponent* AIPerception;

    /** �þ� ���� */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class UAISenseConfig_Sight* SightConfig;

    /** �����̺�� Ʈ�� ���� */
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBehaviorTree* BehaviorTreeAsset;

    /** ������ ���� */
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    class UBlackboardData* BlackboardAsset;

    // ADDED: ����(Investigate) ���� ����(�����Ϳ��� ���� ����)
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float InvestigateTimeout = 3.0f;

private:
    /** �� AI�� �� ID */
    static constexpr uint8 EnemyTeamId = 2;

    // ADDED: ���� Ÿ�̸� �ڵ�
    FTimerHandle InvestigateTimerHandle;

    // ADDED: ���� ���� ó�� �Լ�
    void StopInvestigating();

    // ADDED: �þ߰� ����� �Լ�
    void Debug_DrawFOV();
};