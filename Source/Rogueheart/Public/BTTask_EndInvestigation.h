#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EndInvestigation.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_EndInvestigation : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_EndInvestigation();

    // 수색 상태 키
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector IsInvestigatingKey;

    // 마지막 발견 위치 키 (클리어할 키)
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector DiscoveredLocationKey;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
