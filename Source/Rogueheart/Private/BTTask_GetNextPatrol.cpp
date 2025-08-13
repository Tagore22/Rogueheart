#include "BTTask_GetNextPatrol.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetNextPatrol::UBTTask_GetNextPatrol()
{
    NodeName = TEXT("Get Next Patrol");
    PatrolLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetNextPatrol, PatrolLocationKey));
}

EBTNodeResult::Type UBTTask_GetNextPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    // PatrolPoints �迭�� ��������� ����
    if (EnemyBase->PatrolPoints.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    // ���� ���� �������� �̵�
    EnemyBase->CurrentPatrolIndex = (EnemyBase->CurrentPatrolIndex + 1) % EnemyBase->PatrolPoints.Num();

    AActor* NextPatrolPoint = EnemyBase->PatrolPoints[EnemyBase->CurrentPatrolIndex];
    if (!NextPatrolPoint)
    {
        return EBTNodeResult::Failed;
    }

    // �����忡 ���� ���� ��ġ ����
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(PatrolLocationKey.SelectedKeyName, NextPatrolPoint->GetActorLocation());
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}