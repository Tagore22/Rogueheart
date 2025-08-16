#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetMovementSpeed.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_SetMovementSpeed : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_SetMovementSpeed();

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MovementSpeed = 150.0f;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};