#include "BTTask_FaceTarget.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FaceTarget::UBTTask_FaceTarget()
{
    NodeName = TEXT("Face Target");
    TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FaceTarget, TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_FaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AEnemyBase* EnemyBase = Cast<AEnemyBase>(AIController->GetPawn());
    if (!EnemyBase)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
    if (!TargetActor)
    {
        return EBTNodeResult::Failed;
    }

    // 타겟을 향한 회전 계산
    FVector Direction = TargetActor->GetActorLocation() - EnemyBase->GetActorLocation();
    Direction.Z = 0.0f; // 수평면에서만 회전

    if (Direction.SizeSquared() > 0.0f)
    {
        FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
        EnemyBase->SetActorRotation(TargetRotation);
    }

    return EBTNodeResult::Succeeded;
}