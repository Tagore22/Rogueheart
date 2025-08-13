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

    // ���� ���� Ű
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector IsInvestigatingKey;

    // ������ �߰� ��ġ Ű (Ŭ������ Ű)
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector DiscoveredLocationKey;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
