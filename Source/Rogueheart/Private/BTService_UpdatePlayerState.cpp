#include "BTService_UpdatePlayerState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTService_UpdatePlayerState::UBTService_UpdatePlayerState()
{
    NodeName = TEXT("Update Player State");
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = false;
    Interval = 0.3f;
}

void UBTService_UpdatePlayerState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsBool(TEXT("BoolTrue"), true);
    }
}

void UBTService_UpdatePlayerState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
        return;

    AActor* TargetPlayer = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetPlayer")));
    APawn* ControlledPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;

    if (!TargetPlayer || !ControlledPawn)
        return;

    float Distance = FVector::Dist(TargetPlayer->GetActorLocation(), ControlledPawn->GetActorLocation());

    BlackboardComp->SetValueAsBool(TEXT("bInTargetRange"), Distance <= TargetRangeDistance);
    BlackboardComp->SetValueAsBool(TEXT("bInAttackRange"), Distance <= AttackRangeDistance);

    //
    FVector DiscoveredLocation = BlackboardComp->GetValueAsVector(TEXT("DiscoveredLocation"));
    float DistanceFromChaseStart = FVector::Dist(ControlledPawn->GetActorLocation(), DiscoveredLocation);
    BlackboardComp->SetValueAsBool(TEXT("bShouldReturn"), DistanceFromChaseStart > MaxChaseDistance); // 원래는 <= 였음.
}