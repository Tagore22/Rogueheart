#include "BTTask/BTTask_CirclePlayer.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"

UBTTask_CirclePlayer::UBTTask_CirclePlayer()
{
    NodeName = TEXT("Circle Player");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CirclePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    APawn* ControlledPawn = AICon->GetPawn();
    if (!ControlledPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    AActor* TargetPlayer = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetPlayer")));
    if (!TargetPlayer) return EBTNodeResult::Failed;

    FVector PlayerLocation = TargetPlayer->GetActorLocation();
    FVector ToEnemy = ControlledPawn->GetActorLocation() - PlayerLocation;
    ToEnemy.Z = 0.f;
    ToEnemy.Normalize();

    FVector Right = FVector::CrossProduct(FVector::UpVector, ToEnemy);
    FVector CircleLocation = PlayerLocation + ToEnemy * CircleRadius + Right * MoveDistance;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(ControlledPawn->GetWorld());
    if (NavSys)
    {
        FNavLocation NavLoc;
        if (NavSys->ProjectPointToNavigation(CircleLocation, NavLoc))
        {
            // ��ǥ ��ġ�� �����忡 ����
            BB->SetValueAsVector(TEXT("CircleTargetLocation"), NavLoc.Location);

            AICon->MoveToLocation(NavLoc.Location);
            UE_LOG(LogTemp, Warning, TEXT("Circle"));
            return EBTNodeResult::InProgress; // InProgress�� ����
        }
    }

    return EBTNodeResult::Failed;
}

void UBTTask_CirclePlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    APawn* ControlledPawn = AICon->GetPawn();
    if (!ControlledPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // TargetPlayer�� ������� Task ����
    AActor* TargetPlayer = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetPlayer")));
    if (!TargetPlayer)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // ��ǥ ������ �����ߴ��� üũ
    FVector TargetLocation = BB->GetValueAsVector(TEXT("CircleTargetLocation"));
    float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), TargetLocation);

    if (Distance <= AcceptanceRadius)
    {
        // ��ǥ�� ���������Ƿ� Task �Ϸ�
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}