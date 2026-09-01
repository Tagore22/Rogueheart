#include "BTTask/BTTask_CalculateLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "SkillNames.h"

EBTNodeResult::Type UBTTask_CalculateLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    APawn* ControlledPawn = AICon->GetPawn();
    if (!ControlledPawn)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    AActor* TargetPlayer = Cast<AActor>(BB->GetValueAsObject(SkillNames::TargetPlayer));
    if (!TargetPlayer) return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(ControlledPawn->GetWorld());
    if (NavSys)
    {
        FVector PlayerLocation = TargetPlayer->GetActorLocation();

        float ToEnemy = FVector::Dist(PlayerLocation, ControlledPawn->GetActorLocation());

        // ÀÓ½Ã°ª.
        float Radius = ToEnemy - 20.f;

        FNavLocation RandomLocation;

        NavSys->GetRandomReachablePointInRadius(
            TargetPlayer->GetActorLocation(),
            Radius,
            RandomLocation
        );
        BB->SetValueAsVector(SkillNames::CircleLocation, RandomLocation);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}