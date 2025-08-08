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

    /** �� ID ��ȯ */
    virtual FGenericTeamId GetGenericTeamId() const override;

protected:
    virtual void BeginPlay() override;

    /** ���� �̺�Ʈ ó�� */
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
    /** AI Perception ������Ʈ */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UAIPerceptionComponent* AIPerception;

    /** �þ� ���� */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UAISenseConfig_Sight* SightConfig;

    /** ���� �Ÿ� */
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackDistance = 200.f;

    /** ���� Ÿ�� */
    UPROPERTY()
    AActor* TargetActor;

    /** �� AI�� �� ID */
    static constexpr uint8 EnemyTeamId = 2;
};