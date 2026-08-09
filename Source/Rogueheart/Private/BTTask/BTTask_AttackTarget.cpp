#include "BTTask/BTTask_AttackTarget.h"
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

    AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
    if (!Enemy)
    {
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Warning, TEXT("Attack Ready!"));
    // 공격 실행
    Enemy->TryAttack();

    // LastAttackTime 업데이트 (향후 확장용)
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsFloat(TEXT("LastAttackTime"), GetWorld()->GetTimeSeconds());
    }

    return EBTNodeResult::Succeeded;
}