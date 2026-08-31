#include "BTTask/BTTask_SetChaseMovespeed.h"
#include "SkillNames.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"

EBTNodeResult::Type UBTTask_SetChaseMovespeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    EnemyBase->SetMaxWalkSpeed(SkillNames::ChaseSpeed);

    return EBTNodeResult::Succeeded;
}