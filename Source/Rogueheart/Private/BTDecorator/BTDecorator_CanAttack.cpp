#include "BTDecorator/BTDecorator_CanAttack.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
    NodeName = TEXT("Can Attack");

    // 최적화: 틱 기반 체크가 아닌 이벤트 기반으로 설정
    bNotifyTick = false;
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = false;
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // AI Controller 체크
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return false;
    }

    // EnemyBase 캐스팅
    AEnemyBase* EnemyBase = Cast<AEnemyBase>(AIController->GetPawn());
    if (!EnemyBase)
    {
        return false;
    }

    // 공격 가능 여부 반환
    return EnemyBase->CanAttack();
}

FString UBTDecorator_CanAttack::GetStaticDescription() const
{
    return TEXT("Can Attack");
}