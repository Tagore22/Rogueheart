#include "BTTask/BTTask_LookAround.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_LookAround::UBTTask_LookAround()
{
    NodeName = TEXT("Look Around");
    bNotifyTick = true; // Tick 이벤트 활성화
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

    // 메모리 초기화
    FLookAroundMemory* Memory = reinterpret_cast<FLookAroundMemory*>(NodeMemory);
    Memory->Reset();
    Memory->InitialRotation = EnemyBase->GetActorRotation();

    // 첫 번째 목표 각도 설정 (오른쪽으로)
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

    // 시간 초과 시 종료
    if (Memory->ElapsedTime >= LookAroundDuration)
    {
        // 원래 방향으로 복귀
        EnemyBase->SetActorRotation(Memory->InitialRotation);
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    FRotator CurrentRotation = EnemyBase->GetActorRotation();
    FRotator NewRotation = UKismetMathLibrary::RInterpTo(
        CurrentRotation,
        Memory->TargetRotation,
        DeltaSeconds,
        RotationSpeed / 180.0f // 초당 회전 속도를 보간 속도로 변환
    );

    EnemyBase->SetActorRotation(NewRotation);

    // 목표 각도에 도달했는지 확인 (5도 이내)
    float AngleDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(NewRotation.Yaw, Memory->TargetRotation.Yaw));
    if (AngleDifference < 5.0f)
    {
        if (Memory->bTurningRight && !Memory->bReturningToCenter)
        {
            // 오른쪽 완료, 이제 왼쪽으로
            Memory->TargetRotation = Memory->InitialRotation;
            Memory->TargetRotation.Yaw -= LookAroundAngle;
            Memory->bTurningRight = false;
        }
        else if (!Memory->bTurningRight && !Memory->bReturningToCenter)
        {
            // 왼쪽 완료, 이제 중앙으로
            Memory->TargetRotation = Memory->InitialRotation;
            Memory->bReturningToCenter = true;
        }
    }
}

FString UBTTask_LookAround::GetStaticDescription() const
{
    return FString::Printf(TEXT("Look around (%.1fs, ±%.0f°)"), LookAroundDuration, LookAroundAngle);
}