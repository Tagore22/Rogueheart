#include "BTTask_SetMovementSpeed.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed()
{
    NodeName = TEXT("Set Movement Speed");
}

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    UCharacterMovementComponent* MovementComp = EnemyBase->GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->MaxWalkSpeed = MovementSpeed;
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}