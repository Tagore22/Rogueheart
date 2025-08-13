#include "BTTask_AttackTarget.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
    NodeName = TEXT("Attack Target");
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AEnemyBase* EnemyBase = Cast<AEnemyBase>(AIController->GetPawn());
    if (!EnemyBase)
    {
        return EBTNodeResult::Failed;
    }

    // ���� ����
    EnemyBase->TryAttack();

    // LastAttackTime ������Ʈ (���� Ȯ���)
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsFloat(TEXT("LastAttackTime"), GetWorld()->GetTimeSeconds());
    }

    return EBTNodeResult::Succeeded;
}