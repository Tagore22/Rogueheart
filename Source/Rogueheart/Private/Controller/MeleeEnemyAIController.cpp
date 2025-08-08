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

        // UseBlackboard는 BlackboardComponent를 초기화해줌
        if (UseBlackboard(BlackboardAsset, OutBlackboardComponent))
        {
            // BlackboardComponent 멤버에 할당 (부모 AAIController가 가지고 있음)
            Blackboard = OutBlackboardComponent;

            UE_LOG(LogTemp, Warning, TEXT("Beha Start"));
            RunBehaviorTree(BehaviorTreeAsset);
        }
    }
}