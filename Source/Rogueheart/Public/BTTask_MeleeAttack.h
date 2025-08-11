#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MeleeAttack.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_MeleeAttack : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_MeleeAttack();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "AI")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackDuration = 2.0f; // 애니메이션이 없을 때 기본 공격 시간
};