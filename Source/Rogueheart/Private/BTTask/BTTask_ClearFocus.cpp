#include "BTTask/BTTask_ClearFocus.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    AICon->ClearFocus(EAIFocusPriority::Gameplay);

    ACharacter* Me = Cast<ACharacter>(AICon->GetPawn());
    if (!Me)
    {
        return EBTNodeResult::Failed;
    }

    Me->GetCharacterMovement()->bOrientRotationToMovement = true;
    Me->bUseControllerRotationYaw = true;

    return EBTNodeResult::Succeeded;
}

