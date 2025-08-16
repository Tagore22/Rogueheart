#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WaitIdle.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_WaitIdle : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_WaitIdle();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

    virtual uint16 GetInstanceMemorySize() const override;

    UPROPERTY(EditAnywhere, Category = "Wait")
    float WaitTime = 3.0f;

private:
    struct FWaitIdleTaskMemory
    {
        float ElapsedTime = 0.0f;
    };
};