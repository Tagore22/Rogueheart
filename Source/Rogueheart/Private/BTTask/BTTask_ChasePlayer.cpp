#include "BTTask/BTTask_ChasePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
    NodeName = TEXT("Chase Player");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    AActor* TargetPlayer = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetPlayer")));
    if (!TargetPlayer) return EBTNodeResult::Failed;

    // DiscoveredLocation 설정
    APawn* ControlledPawn = AICon->GetPawn();
    if (ControlledPawn)
    {
        FVector ChaseStartLocation = ControlledPawn->GetActorLocation();
        BB->SetValueAsVector(TEXT("DiscoveredLocation"), ChaseStartLocation);
    }

    UE_LOG(LogTemp, Warning, TEXT("Chase"));
    AICon->MoveToActor(TargetPlayer, AcceptanceRadius, true, true, false, 0, true);
    return EBTNodeResult::InProgress;
}

void UBTTask_ChasePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // TargetPlayer가 없으면 Task 종료
    AActor* TargetPlayer = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetPlayer")));
    if (!TargetPlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("Chase Task: TargetPlayer cleared, finishing task"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // 플레이어에게 도달했는지 체크 (선택사항)
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (AICon && AICon->GetPawn())
    {
        float Distance = FVector::Dist(AICon->GetPawn()->GetActorLocation(), TargetPlayer->GetActorLocation());
        if (Distance <= AcceptanceRadius)
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    }
}