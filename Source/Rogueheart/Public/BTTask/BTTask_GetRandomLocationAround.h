#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetRandomLocationAround.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_GetRandomLocationAround : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_GetRandomLocationAround();

    // �߽����� �� ��ġ Ű (���� DiscoveredLocation)
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector CenterLocationKey;

    // ����� ������ ��ġ Ű (���� RandomLocation)
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector ResultLocationKey;

    // ���� �ݰ�
    UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = "50.0", ClampMax = "1000.0"))
    float SearchRadius = 300.0f;

    // �ִ� ��õ� Ƚ�� (�׺���̼� ���� ��)
    UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = "1", ClampMax = "10"))
    int32 MaxRetries = 5;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;
};