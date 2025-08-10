#include "BTTask_MeleeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
    NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    ACharacter* ControlledChar = Cast<ACharacter>(AICon->GetPawn());
    if (!ControlledChar) return EBTNodeResult::Failed;

    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = ControlledChar->GetMesh()->GetAnimInstance();
        if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
        {
            AnimInstance->Montage_Play(AttackMontage);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Attack"));
    return EBTNodeResult::Succeeded;
}