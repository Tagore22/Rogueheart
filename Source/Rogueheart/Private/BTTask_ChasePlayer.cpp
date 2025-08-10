#include "BTTask_ChasePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
    NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    AActor* TargetPlayer = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetPlayer")));
    if (!TargetPlayer) return EBTNodeResult::Failed;

    //
    APawn* ControlledPawn = AICon->GetPawn();
    if (ControlledPawn)
    {
        FVector ChaseStartLocation = ControlledPawn->GetActorLocation();
        BB->SetValueAsVector(TEXT("DiscoveredLocation"), ChaseStartLocation);
    }
    //
    UE_LOG(LogTemp, Warning, TEXT("Chase"));
    AICon->MoveToActor(TargetPlayer, AcceptanceRadius, true, true, false, 0, true);
    return EBTNodeResult::InProgress;
}