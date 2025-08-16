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
    return EBTNodeResult::InProgress; // InProgress�� ����
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

    // �ִϸ��̼��� ������ �ִϸ��̼� �Ϸ� üũ
    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = ControlledChar->GetMesh()->GetAnimInstance();
        if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
        {
            // �ִϸ��̼� �Ϸ�
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return;
        }
    }
    else
    {
        // �ִϸ��̼��� ������ ���� �ð� �� �Ϸ� (Ÿ�̸� ���� �ʿ�)
        // ������ �ٷ� �Ϸ�� ó��
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}