#include "BTDecorator_HasValidPatrolPoint.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"

UBTDecorator_HasValidPatrolPoint::UBTDecorator_HasValidPatrolPoint()
{
    NodeName = TEXT("Has Valid Patrol Point");

    // 최적화: 이벤트 기반으로 설정
    bNotifyTick = false;
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = false;
}

bool UBTDecorator_HasValidPatrolPoint::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // AI Controller 유효성 체크
    const AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return false;
    }

    // EnemyBase 캐스팅 및 유효성 체크
    const AEnemyBase* EnemyBase = Cast<AEnemyBase>(AIController->GetPawn());
    if (!EnemyBase)
    {
        return false;
    }

    // PatrolPoints 배열이 비어있는지 체크
    if (EnemyBase->PatrolPoints.Num() == 0)
    {
        return false;
    }

    // 현재 인덱스가 유효한 범위 내에 있는지 체크
    if (EnemyBase->CurrentPatrolIndex < 0 ||
        EnemyBase->CurrentPatrolIndex >= EnemyBase->PatrolPoints.Num())
    {
        return false;
    }

    // 현재 인덱스의 PatrolPoint가 유효한 객체인지 체크
    return IsValid(EnemyBase->PatrolPoints[EnemyBase->CurrentPatrolIndex]);
}

FString UBTDecorator_HasValidPatrolPoint::GetStaticDescription() const
{
    return TEXT("Has Valid Patrol Point");
}