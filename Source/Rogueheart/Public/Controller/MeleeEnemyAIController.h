#pragma once

#include "CoreMinimal.h"
#include "Controller/EnemyAIController.h"
#include "MeleeEnemyAIController.generated.h"

class UBehaviorTree;
class UBlackboardData;

UCLASS()
class ROGUEHEART_API AMeleeEnemyAIController : public AEnemyAIController
{
    GENERATED_BODY()

public:
    AMeleeEnemyAIController();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TArray<AActor*> PatrolPoints;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBlackboardData* BlackboardAsset;
};