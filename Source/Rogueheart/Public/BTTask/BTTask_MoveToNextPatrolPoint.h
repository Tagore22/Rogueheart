#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToNextPatrolPoint.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_MoveToNextPatrolPoint : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_MoveToNextPatrolPoint();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FName CurrentPatrolIndexKey = "CurrentPatrolIndex";
};