#include "BTTask/BTTask_GetRandomLocationAround.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Engine/World.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_GetRandomLocationAround::UBTTask_GetRandomLocationAround()
{
    NodeName = TEXT("Get Random Location Around");

    // Ű Ÿ�� ���� ����
    CenterLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetRandomLocationAround, CenterLocationKey));
    CenterLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetRandomLocationAround, CenterLocationKey), AActor::StaticClass());

    ResultLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetRandomLocationAround, ResultLocationKey));
}

EBTNodeResult::Type UBTTask_GetRandomLocationAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
        return EBTNodeResult::Failed;
    }

    // �߽��� ��������
    FVector CenterLocation = FVector::ZeroVector;

    if (CenterLocationKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
    {
        CenterLocation = BlackboardComp->GetValueAsVector(CenterLocationKey.SelectedKeyName);
    }
    else if (CenterLocationKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
    {
        if (AActor* Actor = Cast<AActor>(BlackboardComp->GetValueAsObject(CenterLocationKey.SelectedKeyName)))
        {
            CenterLocation = Actor->GetActorLocation();
        }
    }
    else
    {
        // Ű�� �������� �ʾ����� AI �ڽ��� ��ġ ���
        if (APawn* Pawn = AIController->GetPawn())
        {
            CenterLocation = Pawn->GetActorLocation();
        }
    }

    // ���� ��ġ ã�� (��õ� ����)
    for (int32 i = 0; i < MaxRetries; ++i)
    {
        FNavLocation RandomLocation;
        bool bFound = NavSystem->GetRandomReachablePointInRadius(
            CenterLocation,
            SearchRadius,
            RandomLocation
        );

        if (bFound)
        {
            // ����� �����忡 ����
            BlackboardComp->SetValueAsVector(ResultLocationKey.SelectedKeyName, RandomLocation.Location);

            UE_LOG(LogTemp, Log, TEXT("Found random location around %s at %s"),
                *CenterLocation.ToString(), *RandomLocation.Location.ToString());

            return EBTNodeResult::Succeeded;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Failed to find random location around %s after %d retries"),
        *CenterLocation.ToString(), MaxRetries);

    return EBTNodeResult::Failed;
}

FString UBTTask_GetRandomLocationAround::GetStaticDescription() const
{
    return FString::Printf(TEXT("Get random location (Radius: %.0f)"), SearchRadius);
}