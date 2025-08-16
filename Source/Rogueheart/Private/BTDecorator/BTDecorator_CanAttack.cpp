#include "BTDecorator/BTDecorator_CanAttack.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
    NodeName = TEXT("Can Attack");

    // ����ȭ: ƽ ��� üũ�� �ƴ� �̺�Ʈ ������� ����
    bNotifyTick = false;
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = false;
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // AI Controller üũ
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return false;
    }

    // EnemyBase ĳ����
    AEnemyBase* EnemyBase = Cast<AEnemyBase>(AIController->GetPawn());
    if (!EnemyBase)
    {
        return false;
    }

    // ���� ���� ���� ��ȯ
    return EnemyBase->CanAttack();
}

FString UBTDecorator_CanAttack::GetStaticDescription() const
{
    return TEXT("Can Attack");
}