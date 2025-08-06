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

    UPROPERTY(EditAnywhere, Category = "AI")
    UAnimMontage* AttackMontage;
};