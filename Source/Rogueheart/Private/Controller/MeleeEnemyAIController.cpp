#include "Controller/MeleeEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AMeleeEnemyAIController::AMeleeEnemyAIController()
{   
    // BB와 BT의 경로 수정 요망.
    /*static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/AI/BT_MeleeEnemy"));
    if (BT.Succeeded())
    {
        BehaviorTreeAsset = BT.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Game/AI/BB_MeleeEnemy"));
    if (BB.Succeeded())
    {
        BlackboardAsset = BB.Object;
    }*/
}

void AMeleeEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    /*if (UseBlackboard(BlackboardAsset, Blackboard))
    {
        RunBehaviorTree(BehaviorTreeAsset);
    }*/
}