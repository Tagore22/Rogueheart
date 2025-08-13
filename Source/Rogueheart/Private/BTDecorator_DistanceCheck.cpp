#include "BTDecorator_DistanceCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTDecorator_DistanceCheck::UBTDecorator_DistanceCheck()
{
    NodeName = TEXT("Distance Check");

    // Blackboard Key 필터
    FirstLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_DistanceCheck, FirstLocationKey));
    FirstLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_DistanceCheck, FirstLocationKey), AActor::StaticClass());

    SecondLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_DistanceCheck, SecondLocationKey));
    SecondLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_DistanceCheck, SecondLocationKey), AActor::StaticClass());

    // 실무 추천: Blackboard Observer + Observer Aborts
    bNotifyBecomeRelevant = true;   // Decorator 시작 시 알림
    bNotifyCeaseRelevant = false;   // 종료 시 알림은 불필요
    bNotifyTick = false;            // 매 틱 호출 불필요

    // Observer 설정
    SetIsInversed(false);
    FlowAbortMode = EBTFlowAbortMode::Self;  // 자기 서브트리만 중단
}

bool UBTDecorator_DistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    FVector FirstLocation = FVector::ZeroVector;
    FVector SecondLocation = FVector::ZeroVector;

    // 첫 번째 위치
    if (FirstLocationKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
    {
        FirstLocation = BlackboardComp->GetValueAsVector(FirstLocationKey.SelectedKeyName);
    }
    else if (FirstLocationKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
    {
        if (AActor* Actor = Cast<AActor>(BlackboardComp->GetValueAsObject(FirstLocationKey.SelectedKeyName)))
        {
            FirstLocation = Actor->GetActorLocation();
        }
    }
    else
    {
        if (AAIController* AIController = OwnerComp.GetAIOwner())
        {
            if (APawn* Pawn = AIController->GetPawn())
            {
                FirstLocation = Pawn->GetActorLocation();
            }
        }
    }

    // 두 번째 위치
    if (SecondLocationKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
    {
        SecondLocation = BlackboardComp->GetValueAsVector(SecondLocationKey.SelectedKeyName);
    }
    else if (SecondLocationKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
    {
        if (AActor* Actor = Cast<AActor>(BlackboardComp->GetValueAsObject(SecondLocationKey.SelectedKeyName)))
        {
            SecondLocation = Actor->GetActorLocation();
        }
        else
        {
            return false;
        }
    }

    // 거리 계산
    float Distance = CalculateDistance(FirstLocation, SecondLocation);

    // 조건 판단
    bool bInRange = (Distance >= MinDistance && Distance <= MaxDistance);
    return bInvertCondition ? !bInRange : bInRange;
}

FString UBTDecorator_DistanceCheck::GetStaticDescription() const
{
    return FString::Printf(TEXT("Distance %s %.1f - %.1f (%s)"),
        bInvertCondition ? TEXT("NOT between") : TEXT("between"),
        MinDistance, MaxDistance,
        DistanceType == EDistanceCheckType::Distance2D ? TEXT("2D") : TEXT("3D"));
}

float UBTDecorator_DistanceCheck::CalculateDistance(const FVector& LocationA, const FVector& LocationB) const
{
    return (DistanceType == EDistanceCheckType::Distance2D)
        ? FVector::Dist2D(LocationA, LocationB)
        : FVector::Dist(LocationA, LocationB);
}