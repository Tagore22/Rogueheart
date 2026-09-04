#include "BTDecorator/BTDecorator_CheckLockonDist.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/EnemyBase.h"

UBTDecorator_CheckLockonDist::UBTDecorator_CheckLockonDist()
{
    bNotifyTick = true;
}

bool UBTDecorator_CheckLockonDist::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

    AEnemyBase* Pawn = Cast<AEnemyBase>(AIController->GetPawn());
    if (!IsValid(Pawn))
    {
        return false;
    }
    FVector EnemyLocation = Pawn->GetActorLocation();

    float MinDistance = BlackboardComp->GetValueAsFloat(MinDistanceKey.SelectedKeyName);
    MinDistance *= MinDistance;

    float MaxDistance = BlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
    MaxDistance *= MaxDistance;

    float DistSquared = FVector::DistSquared(PlayerLocation, EnemyLocation);

    // 스폰위치로부터 일정거리 이상 벗어났거나, 플레이어를 N초 이상 감지하지 못하였을 경우.
    if (DistSquared > MaxDistance || DistSquared <= MinDistance)
    {
        //Pawn->SetbIsLockon(false);
        return false;
    }

    //Pawn->SetbIsLockon(true);
    return true;
}

void UBTDecorator_CheckLockonDist::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

    AEnemyBase* Pawn = Cast<AEnemyBase>(AIController->GetPawn());
    if (!IsValid(Pawn))
    {
        return;
    }
    FVector EnemyLocation = Pawn->GetActorLocation();

    float MinDistance = BlackboardComp->GetValueAsFloat(MinDistanceKey.SelectedKeyName);
    MinDistance *= MinDistance;

    float MaxDistance = BlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
    MaxDistance *= MaxDistance;

    float DistSquared = FVector::DistSquared(PlayerLocation, EnemyLocation);

    // 스폰위치로부터 일정거리 이상 벗어났거나, 플레이어를 N초 이상 감지하지 못하였을 경우.
    if (DistSquared > MaxDistance || DistSquared <= MinDistance)
    {
        OwnerComp.RequestExecution(this);
        //Pawn->SetbIsLockon(false);
        return;
    }
    //Pawn->SetbIsLockon(true);
}

