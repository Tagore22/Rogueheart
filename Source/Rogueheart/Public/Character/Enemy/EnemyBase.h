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

    /** ���� �õ� */
    void TryAttack();

    /** Ÿ�� ��Ŀ ǥ��/����� */
    void ShowTargetMarker(bool bShow);
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** AI Perception�� ���� ���¸� ������Ʈ�� �� ȣ�� */
    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    /** �÷��̾ ���� �̵� */
    void MoveToPlayer();

private:
    /** ���� �÷��̾ ���� ������ ���� */
    bool bHasPerceivedPlayer = false;

    /** ������ ���� ���� ��� �ð� */
    float TimeSinceLastAttack = 0.f;

public:
    /** ���� ���� */
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRange = 150.f;

    /** ���� ��Ÿ�� */
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackCooldown = 2.f;

    /** ���� ��� */
    UPROPERTY(EditAnywhere, Category = "AI")
    UAnimMontage* AttackMontage;

    /** Ÿ�� ��Ŀ UI */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* TargetMarker;

    /** ���� ���� ���� �÷��̾� */
    UPROPERTY()
    APawn* TargetPlayer;

    /** ���� ����Ʈ�� */
    UPROPERTY(EditInstanceOnly, Category = "AI|Patrol")
    TArray<AActor*> PatrolPoints;

    /** ���� ���� �ε��� */
    UPROPERTY(VisibleAnywhere, Category = "AI|Patrol")
    int32 CurrentPatrolIndex = 0;
};