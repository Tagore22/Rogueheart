#include "BTTask_WaitIdle.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_WaitIdle::UBTTask_WaitIdle()
{
    NodeName = TEXT("Wait Idle");
    bNotifyTick = true;
    bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_WaitIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    FWaitIdleTaskMemory* MyMemory = (FWaitIdleTaskMemory*)NodeMemory;
    MyMemory->ElapsedTime = 0.0f;

    return EBTNodeResult::InProgress;
}

void UBTTask_WaitIdle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    FWaitIdleTaskMemory* MyMemory = (FWaitIdleTaskMemory*)NodeMemory;
    MyMemory->ElapsedTime += DeltaSeconds;

    if (MyMemory->ElapsedTime >= WaitTime)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

void UBTTask_WaitIdle::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    // 나중에 필요하면 여기서 상태 초기화 같은 것도 가능
}

uint16 UBTTask_WaitIdle::GetInstanceMemorySize() const
{
    return sizeof(FWaitIdleTaskMemory);
}