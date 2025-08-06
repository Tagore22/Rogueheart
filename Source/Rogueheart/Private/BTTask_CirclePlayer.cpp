#include "BTTask_CirclePlayer.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"

UBTTask_CirclePlayer::UBTTask_CirclePlayer()
{
    NodeName = TEXT("Circle Player");
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
            AICon->MoveToLocation(NavLoc.Location);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}