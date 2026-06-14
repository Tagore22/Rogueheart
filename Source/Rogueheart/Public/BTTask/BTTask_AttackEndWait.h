#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackEndWait.generated.h"

UCLASS()
class UBTTask_AttackEndWait : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_AttackEndWait();

    UPROPERTY(EditDefaultsOnly, Category = "Wait Time")
    float WaitTime = 3.5f;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    float ElapsedTime = 0.f;
};