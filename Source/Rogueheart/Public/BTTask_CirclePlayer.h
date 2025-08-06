#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CirclePlayer.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_CirclePlayer : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_CirclePlayer();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "AI")
    float CircleRadius = 250.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float MoveDistance = 100.f;
};