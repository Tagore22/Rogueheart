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

    /** ���� �õ� (�����̺�� Ʈ������ ȣ��) */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void TryAttack();

    /** Ÿ�� ��Ŀ ǥ��/����� */
    void ShowTargetMarker(bool bShow);

    /** ���� ��ٿ� üũ */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool CanAttack() const;

    /** ������ ���� �ð� ���� */
    void ResetAttackCooldown();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    /** ������ ���� ���� ��� �ð� */
    float TimeSinceLastAttack = 0.f;

public:
    /** ���� ���� */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackRange = 150.f;

    /** ���� ��Ÿ�� */
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackCooldown = 2.f;

    /** ���� ��� */
    UPROPERTY(EditAnywhere, Category = "Combat")
    UAnimMontage* AttackMontage;

    /** Ÿ�� ��Ŀ UI */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* TargetMarker;

    /** ���� ����Ʈ�� */
    UPROPERTY(EditInstanceOnly, Category = "AI|Patrol")
    TArray<AActor*> PatrolPoints;

    /** ���� ���� �ε��� */
    UPROPERTY(VisibleAnywhere, Category = "AI|Patrol")
    int32 CurrentPatrolIndex = 0;
};