#include "Controller/MeleeEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "UObject/ConstructorHelpers.h"

AMeleeEnemyAIController::AMeleeEnemyAIController()
{
    // ���� ���� ���� ������ �θ� ������ �Ҵ�
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Script/AIModule.BehaviorTree'/Game/Characters/BT_EnemyAI.BT_EnemyAI'"));
    if (BT.Succeeded())
    {
        BehaviorTreeAsset = BT.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Script/AIModule.BlackboardData'/Game/Characters/BB_EnemyAI.BB_EnemyAI'"));
    if (BB.Succeeded())
    {
        BlackboardAsset = BB.Object;
    }
}