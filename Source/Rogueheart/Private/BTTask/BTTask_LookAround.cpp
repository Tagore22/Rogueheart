#include "BTTask/BTTask_LookAround.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_LookAround::UBTTask_LookAround()
{
    NodeName = TEXT("Look Around");
    bNotifyTick = true; // Tick �̺�Ʈ Ȱ��ȭ
}

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    // �޸� �ʱ�ȭ
    FLookAroundMemory* Memory = reinterpret_cast<FLookAroundMemory*>(NodeMemory);
    Memory->Reset();
    Memory->InitialRotation = EnemyBase->GetActorRotation();

    // ù ��° ��ǥ ���� ���� (����������)
    Memory->TargetRotation = Memory->InitialRotation;
    Memory->TargetRotation.Yaw += LookAroundAngle;
    Memory->bTurningRight = true;

    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_LookAround::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::Aborted;
}

void UBTTask_LookAround::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AEnemyBase* EnemyBase = Cast<AEnemyBase>(AIController->GetPawn());
    if (!EnemyBase)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FLookAroundMemory* Memory = reinterpret_cast<FLookAroundMemory*>(NodeMemory);
    Memory->ElapsedTime += DeltaSeconds;

    // �ð� �ʰ� �� ����
    if (Memory->ElapsedTime >= LookAroundDuration)
    {
        // ���� �������� ����
        EnemyBase->SetActorRotation(Memory->InitialRotation);
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    FRotator CurrentRotation = EnemyBase->GetActorRotation();
    FRotator NewRotation = UKismetMathLibrary::RInterpTo(
        CurrentRotation,
        Memory->TargetRotation,
        DeltaSeconds,
        RotationSpeed / 180.0f // �ʴ� ȸ�� �ӵ��� ���� �ӵ��� ��ȯ
    );

    EnemyBase->SetActorRotation(NewRotation);

    // ��ǥ ������ �����ߴ��� Ȯ�� (5�� �̳�)
    float AngleDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(NewRotation.Yaw, Memory->TargetRotation.Yaw));
    if (AngleDifference < 5.0f)
    {
        if (Memory->bTurningRight && !Memory->bReturningToCenter)
        {
            // ������ �Ϸ�, ���� ��������
            Memory->TargetRotation = Memory->InitialRotation;
            Memory->TargetRotation.Yaw -= LookAroundAngle;
            Memory->bTurningRight = false;
        }
        else if (!Memory->bTurningRight && !Memory->bReturningToCenter)
        {
            // ���� �Ϸ�, ���� �߾�����
            Memory->TargetRotation = Memory->InitialRotation;
            Memory->bReturningToCenter = true;
        }
    }
}

FString UBTTask_LookAround::GetStaticDescription() const
{
    return FString::Printf(TEXT("Look around (%.1fs, ��%.0f��)"), LookAroundDuration, LookAroundAngle);
}