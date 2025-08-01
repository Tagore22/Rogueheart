#pragma once

#include "CoreMinimal.h"
#include "Controller/EnemyAIController.h"
#include "MeleeEnemyAIController.generated.h"

UCLASS()
class ROGUEHEART_API AMeleeEnemyAIController : public AEnemyAIController
{
    GENERATED_BODY()

public:
    AMeleeEnemyAIController();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBlackboardData* BlackboardAsset;
};