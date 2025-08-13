#include "BTDecorator_HasValidPatrolPoint.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"

UBTDecorator_HasValidPatrolPoint::UBTDecorator_HasValidPatrolPoint()
{
    NodeName = TEXT("Has Valid Patrol Point");

    // ����ȭ: �̺�Ʈ ������� ����
    bNotifyTick = false;
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = false;
}

bool UBTDecorator_HasValidPatrolPoint::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // AI Controller ��ȿ�� üũ
    const AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return false;
    }

    // EnemyBase ĳ���� �� ��ȿ�� üũ
    const AEnemyBase* EnemyBase = Cast<AEnemyBase>(AIController->GetPawn());
    if (!EnemyBase)
    {
        return false;
    }

    // PatrolPoints �迭�� ����ִ��� üũ
    if (EnemyBase->PatrolPoints.Num() == 0)
    {
        return false;
    }

    // ���� �ε����� ��ȿ�� ���� ���� �ִ��� üũ
    if (EnemyBase->CurrentPatrolIndex < 0 ||
        EnemyBase->CurrentPatrolIndex >= EnemyBase->PatrolPoints.Num())
    {
        return false;
    }

    // ���� �ε����� PatrolPoint�� ��ȿ�� ��ü���� üũ
    return IsValid(EnemyBase->PatrolPoints[EnemyBase->CurrentPatrolIndex]);
}

FString UBTDecorator_HasValidPatrolPoint::GetStaticDescription() const
{
    return TEXT("Has Valid Patrol Point");
}