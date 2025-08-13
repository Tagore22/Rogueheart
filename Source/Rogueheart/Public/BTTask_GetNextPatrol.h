#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetNextPatrol.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_GetNextPatrol : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_GetNextPatrol();

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector PatrolLocationKey;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};