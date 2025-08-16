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

    // 중심점이 될 위치 키 (보통 DiscoveredLocation)
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector CenterLocationKey;

    // 결과를 저장할 위치 키 (보통 RandomLocation)
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector ResultLocationKey;

    // 수색 반경
    UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = "50.0", ClampMax = "1000.0"))
    float SearchRadius = 300.0f;

    // 최대 재시도 횟수 (네비게이션 실패 시)
    UPROPERTY(EditAnywhere, Category = "Search", meta = (ClampMin = "1", ClampMax = "10"))
    int32 MaxRetries = 5;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual FString GetStaticDescription() const override;
};