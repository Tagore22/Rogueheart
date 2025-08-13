#include "BTTask_EndInvestigation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_EndInvestigation::UBTTask_EndInvestigation()
{
    NodeName = TEXT("End Investigation");

    // 키 타입 필터 설정
    IsInvestigatingKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EndInvestigation, IsInvestigatingKey));
    DiscoveredLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EndInvestigation, DiscoveredLocationKey));
}

EBTNodeResult::Type UBTTask_EndInvestigation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    // 수색 모드 종료
    BlackboardComp->SetValueAsBool(IsInvestigatingKey.SelectedKeyName, false);

    // 마지막 발견 위치 클리어
    BlackboardComp->ClearValue(DiscoveredLocationKey.SelectedKeyName);

    UE_LOG(LogTemp, Log, TEXT("Investigation ended, returning to patrol"));

    return EBTNodeResult::Succeeded;
}