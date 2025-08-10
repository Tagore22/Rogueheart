#include "Controller/MeleeEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "UObject/ConstructorHelpers.h"

AMeleeEnemyAIController::AMeleeEnemyAIController()
{
    // 근접 전투 전용 에셋을 부모 변수에 할당
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/Characters/BT_MeleeEnemy.BT_MeleeEnemy'"));
    if (BT.Succeeded())
    {
        BehaviorTreeAsset = BT.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Script/AIModule.BlackboardData'/Game/Characters/BB_MeleeEnemy.BB_MeleeEnemy'"));
    if (BB.Succeeded())
    {
        BlackboardAsset = BB.Object;
    }
}