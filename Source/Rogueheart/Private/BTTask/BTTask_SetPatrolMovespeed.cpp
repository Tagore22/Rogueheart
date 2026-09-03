#include "BTTask/BTTask_SetPatrolMovespeed.h"
#include "SkillNames.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"

EBTNodeResult::Type UBTTask_SetPatrolMovespeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    EnemyBase->SetMaxWalkSpeed(SkillNames::PatrolSpeed);

    UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed = PatrolSpeed : %f"), SkillNames::PatrolSpeed);

    return EBTNodeResult::Succeeded;
}