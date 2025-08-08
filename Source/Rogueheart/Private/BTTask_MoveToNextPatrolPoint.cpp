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

    // 현재 인덱스를 바탕으로 다음 포인트 설정
    Enemy->CurrentPatrolIndex = (Enemy->CurrentPatrolIndex + 1) % Enemy->PatrolPoints.Num();
    AActor* NextPatrolTarget = Enemy->PatrolPoints[Enemy->CurrentPatrolIndex];

    if (!NextPatrolTarget)
        return EBTNodeResult::Failed;

    // 이동 명령
    AICon->MoveToActor(NextPatrolTarget, 5.f, true);
    UE_LOG(LogTemp, Warning, TEXT("Patrol"));

    return EBTNodeResult::Succeeded;
}