#include "BTTask/BTTask_MeleeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
    NodeName = TEXT("Melee Attack");
    bNotifyTick = true;
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
    return EBTNodeResult::InProgress; // InProgress로 변경
}

void UBTTask_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    ACharacter* ControlledChar = Cast<ACharacter>(AICon->GetPawn());
    if (!ControlledChar)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // 애니메이션이 있으면 애니메이션 완료 체크
    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = ControlledChar->GetMesh()->GetAnimInstance();
        if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
        {
            // 애니메이션 완료
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return;
        }
    }
    else
    {
        // 애니메이션이 없으면 일정 시간 후 완료 (타이머 구현 필요)
        // 간단히 바로 완료로 처리
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}