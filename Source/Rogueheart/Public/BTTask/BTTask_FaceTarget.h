#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FaceTarget.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_FaceTarget : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_FaceTarget();

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere, Category = "Rotation")
    float RotationSpeed = 500.0f;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};