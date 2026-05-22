#include "BTTask/BTTask_AttackEndWait.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AttackEndWait::UBTTask_AttackEndWait()
{
    bNotifyTick = true; // TickTask 호출을 위해 반드시 필요
}

EBTNodeResult::Type UBTTask_AttackEndWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ElapsedTime = 0.f;
    return EBTNodeResult::InProgress;
}

void UBTTask_AttackEndWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ElapsedTime += DeltaSeconds;

    // 플레이어 바라보기
    APawn* Enemy = OwnerComp.GetAIOwner()->GetPawn();
    AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetPlayer")));

    if (Enemy && Target)
    {
        FVector Dir = (Target->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
        FRotator LookRot = FRotationMatrix::MakeFromX(Dir).Rotator();
        LookRot.Pitch = 0.f;
        Enemy->SetActorRotation(LookRot);
    }

    // 3초 후 완료
    if (ElapsedTime >= WaitTime)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}