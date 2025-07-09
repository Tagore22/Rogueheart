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

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // ����
    void MoveToPlayer();

    // ���� �õ�
    void TryAttack();

    // ���� �Ÿ�
    UPROPERTY(EditAnywhere, Category = "AI")
    float DetectRange = 800.f;

    // ���� �Ÿ�
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRange = 150.f;

    // ���� ��Ÿ��
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackCooldown = 2.f;

    // �ִϸ��̼� ��Ÿ��
    UPROPERTY(EditAnywhere, Category = "AI")
    UAnimMontage* AttackMontage;

    // Ÿ��
    APawn* TargetPlayer;

private:
    float TimeSinceLastAttack = 0.f;
};