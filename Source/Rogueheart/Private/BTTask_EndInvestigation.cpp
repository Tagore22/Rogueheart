#include "BTTask_EndInvestigation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_EndInvestigation::UBTTask_EndInvestigation()
{
    NodeName = TEXT("End Investigation");

    // Ű Ÿ�� ���� ����
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

    // ���� ��� ����
    BlackboardComp->SetValueAsBool(IsInvestigatingKey.SelectedKeyName, false);

    // ������ �߰� ��ġ Ŭ����
    BlackboardComp->ClearValue(DiscoveredLocationKey.SelectedKeyName);

    UE_LOG(LogTemp, Log, TEXT("Investigation ended, returning to patrol"));

    return EBTNodeResult::Succeeded;
}