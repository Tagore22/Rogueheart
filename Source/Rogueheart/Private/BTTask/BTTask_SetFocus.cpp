#include "BTTask/BTTask_SetFocus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTask_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    ACharacter* Me = Cast<ACharacter>(AICon->GetPawn());
    if (!Me)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    AActor* TargetPlayer = Cast<AActor>(BB->GetValueAsObject(TargetPlayerKey.SelectedKeyName));
    if (!TargetPlayer)
    {
        return EBTNodeResult::Failed;
    }

    Me->GetCharacterMovement()->bOrientRotationToMovement = false;
    Me->bUseControllerRotationYaw = false;

    AICon->SetFocus(TargetPlayer);

    return EBTNodeResult::Succeeded;
}