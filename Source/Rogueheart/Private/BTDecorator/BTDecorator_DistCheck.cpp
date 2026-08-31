#include "BTDecorator/BTDecorator_DistCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_DistCheck::UBTDecorator_DistCheck()
{
    bNotifyTick = true;
}

/*bool UBTDecorator_DistCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    FVector SpawnLocation = BlackboardComp->GetValueAsVector(FirstLocationKey.SelectedKeyName);
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return false;
    }

    APawn * Pawn = AIController->GetPawn();
    if (!IsValid(Pawn))
    {
        return false;
    }
    FVector CurLocation = Pawn->GetActorLocation();

    float MaxDistance = BlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
    MaxDistance *= MaxDistance;

    float LastPerceptionTime = BlackboardComp->GetValueAsFloat(LastPerceptionTimeKey.SelectedKeyName);

    float CurrentTime = OwnerComp.GetWorld()->GetTimeSeconds();

    float LostTargetTime = BlackboardComp->GetValueAsFloat(LostTargetTimeKey.SelectedKeyName);

    if (FVector::DistSquared(SpawnLocation, CurLocation) > MaxDistance || CurrentTime - LastPerceptionTime > LostTargetTime)
    {
        BlackboardComp->SetValueAsObject(TargetPlayerKey.SelectedKeyName, nullptr);
        return false;
    }
    
    return true;
}*/

void UBTDecorator_DistCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    FVector SpawnLocation = BlackboardComp->GetValueAsVector(SpawnLocationKey.SelectedKeyName);
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
    FVector CurLocation = Pawn->GetActorLocation();

    float MaxDistance = BlackboardComp->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
    MaxDistance *= MaxDistance;

    float LastPerceptionTime = BlackboardComp->GetValueAsFloat(LastPerceptionTimeKey.SelectedKeyName);

    float CurrentTime = OwnerComp.GetWorld()->GetTimeSeconds(); 
    
    float LostTargetTime = BlackboardComp->GetValueAsFloat(LostTargetTimeKey.SelectedKeyName);

    bool bIsPerception = BlackboardComp->GetValueAsBool(bIsPerceptionKey.SelectedKeyName);

    UE_LOG(LogTemp, Warning, TEXT("LastPerceptionTime : %f, Miss Time : %f"), LastPerceptionTime, CurrentTime - LastPerceptionTime);
    //UE_LOG(LogTemp, Warning, TEXT("LastPerceptionTime : %f"), LastPerceptionTime);
    //UE_LOG(LogTemp, Warning, TEXT("DistSquared : %f, MaxDistance : %f, SpawnLocation : %f, %f, %f"), FVector::DistSquared(SpawnLocation, CurLocation), MaxDistance, SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

    // 스폰위치로부터 일정거리 이상 벗어났거나, 플레이어를 N초 이상 감지하지 못하였을 경우.
    if ((FVector::DistSquared(SpawnLocation, CurLocation) > MaxDistance) || (!bIsPerception && (CurrentTime - LastPerceptionTime > LostTargetTime)))
    {
        // 여기서 TargetPlayer를 nullptr로 만들고 패트롤로 되돌아간다.
        BlackboardComp->SetValueAsObject(TargetPlayerKey.SelectedKeyName, nullptr);
        OwnerComp.RequestExecution(this);
        return;
    }
}