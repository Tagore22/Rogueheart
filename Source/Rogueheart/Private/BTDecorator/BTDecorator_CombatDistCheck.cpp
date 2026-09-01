#include "BTDecorator/BTDecorator_CombatDistCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CombatDistCheck::UBTDecorator_CombatDistCheck()
{
    bNotifyTick = true;
}

bool UBTDecorator_CombatDistCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetPlayerKey.SelectedKeyName));
    if (!IsValid(Target))
    {
        return false;
    }
    FVector PlayerLocation = Target->GetActorLocation();

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return false;
    }

    APawn* Pawn = AIController->GetPawn();
    if (!IsValid(Pawn))
    {
        return false;
    }
    FVector EnemyLocation = Pawn->GetActorLocation();

    float MaxDistance = BlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
    MaxDistance *= MaxDistance;

    // 스폰위치로부터 일정거리 이상 벗어났거나, 플레이어를 N초 이상 감지하지 못하였을 경우.
    if (FVector::DistSquared(PlayerLocation, EnemyLocation) <= MaxDistance)
    {
        return false;
    }

    return true;
}

void UBTDecorator_CombatDistCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetPlayerKey.SelectedKeyName));
    if (!IsValid(Target))
    {
        return;
    }
    FVector PlayerLocation = Target->GetActorLocation();

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return;
    }

    APawn* Pawn = AIController->GetPawn();
    if (!IsValid(Pawn))
    {
        return;
    }
    FVector EnemyLocation = Pawn->GetActorLocation();

    float MaxDistance = BlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
    MaxDistance *= MaxDistance;

    // 스폰위치로부터 일정거리 이상 벗어났거나, 플레이어를 N초 이상 감지하지 못하였을 경우.
    if (FVector::DistSquared(PlayerLocation, EnemyLocation) <= MaxDistance)
    {
        OwnerComp.RequestExecution(this);
        return;
    }
}
