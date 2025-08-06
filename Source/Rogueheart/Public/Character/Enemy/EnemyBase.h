#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UWidgetComponent;

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

    void MoveToPlayer();

    void TryAttack();

    void ShowTargetMarker(bool bShow);

public:
    UPROPERTY(EditAnywhere, Category = "AI")
    float DetectRange = 800.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRange = 150.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackCooldown = 2.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    UAnimMontage* AttackMontage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* TargetMarker;

    UPROPERTY()
    APawn* TargetPlayer;

    UPROPERTY(EditInstanceOnly, Category = "AI|Patrol")
    TArray<AActor*> PatrolPoints;

    UPROPERTY(VisibleAnywhere, Category = "AI|Patrol")
    int32 CurrentPatrolIndex = 0;

private:
    float TimeSinceLastAttack = 0.f;
};