#include "BTTask_MoveToNextPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"

UBTTask_MoveToNextPatrolPoint::UBTTask_MoveToNextPatrolPoint()
{
    NodeName = TEXT("Move To Next Patrol Point");
}

EBTNodeResult::Type UBTTask_MoveToNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
        return EBTNodeResult::Failed;

    AEnemyBase* Enemy = Cast<AEnemyBase>(AICon->GetPawn());
    if (!Enemy || Enemy->PatrolPoints.Num() == 0)
        return EBTNodeResult::Failed;

    // ���� �ε����� �������� ���� ����Ʈ ����
    Enemy->CurrentPatrolIndex = (Enemy->CurrentPatrolIndex + 1) % Enemy->PatrolPoints.Num();
    AActor* NextPatrolTarget = Enemy->PatrolPoints[Enemy->CurrentPatrolIndex];

    if (!NextPatrolTarget)
        return EBTNodeResult::Failed;

    // �̵� ���
    AICon->MoveToActor(NextPatrolTarget, 5.f, true);
    UE_LOG(LogTemp, Warning, TEXT("Patrol"));

    return EBTNodeResult::Succeeded;
}