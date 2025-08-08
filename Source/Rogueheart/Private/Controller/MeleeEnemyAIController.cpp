#include "Controller/MeleeEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"

AMeleeEnemyAIController::AMeleeEnemyAIController()
{
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

void AMeleeEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    if (BlackboardAsset && BehaviorTreeAsset)
    {
        UBlackboardComponent* OutBlackboardComponent = nullptr;

        // UseBlackboard�� BlackboardComponent�� �ʱ�ȭ����
        if (UseBlackboard(BlackboardAsset, OutBlackboardComponent))
        {
            // BlackboardComponent ����� �Ҵ� (�θ� AAIController�� ������ ����)
            Blackboard = OutBlackboardComponent;

            UE_LOG(LogTemp, Warning, TEXT("Beha Start"));
            RunBehaviorTree(BehaviorTreeAsset);
        }
    }
}