#include "BTDecorator_IsInvestigating.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInvestigating::UBTDecorator_IsInvestigating()
{
    NodeName = TEXT("Is Investigating");

    // Blackboard Key ���� ����
    IsInvestigatingKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInvestigating, IsInvestigatingKey));

    // UE 5.1 ���� Observer ����
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = true;
    bNotifyTick = false;

    // FlowAbortMode ���� - Ű ���� �� ���򰡸� ����
    FlowAbortMode = EBTFlowAbortMode::Self;
}

bool UBTDecorator_IsInvestigating::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // ������ ������Ʈ ��ȿ�� üũ
    const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    // Ű�� �����Ǿ� �ִ��� üũ
    if (IsInvestigatingKey.SelectedKeyName == NAME_None)
    {
        return false;
    }

    // �����忡�� Boolean �� ��������
    return BlackboardComp->GetValueAsBool(IsInvestigatingKey.SelectedKeyName);
}

FString UBTDecorator_IsInvestigating::GetStaticDescription() const
{
    return FString::Printf(TEXT("Is Investigating: %s"),
        IsInvestigatingKey.SelectedKeyName != NAME_None ?
        *IsInvestigatingKey.SelectedKeyName.ToString() : TEXT("None"));
}