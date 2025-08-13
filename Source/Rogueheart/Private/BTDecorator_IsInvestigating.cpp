#include "BTDecorator_IsInvestigating.h"
#include "BehaviorTree/BlackboardComponent.h"

/*UBTDecorator_IsInvestigating::UBTDecorator_IsInvestigating()
{
    NodeName = TEXT("Is Investigating");

    // Blackboard Key ���� ����
    IsInvestigatingKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInvestigating, IsInvestigatingKey));

    // Observer ���� - ������ Ű ���� �� �ڵ����� ����
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = true;
    bNotifyTick = false;

    // ������ Ű ���� ���� Ȱ��ȭ
    SetIsObservingBlackboard(true);
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

void UBTDecorator_IsInvestigating::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);

    // ������ Ű ���� ���� ����
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp && IsInvestigatingKey.SelectedKeyName != NAME_None)
    {
        BlackboardComp->RegisterObserver(IsInvestigatingKey.GetSelectedKeyID(), this,
            FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_IsInvestigating::OnBlackboardKeyValueChange));
    }
}

void UBTDecorator_IsInvestigating::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // ������ Ű ���� ���� ����
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp && IsInvestigatingKey.SelectedKeyName != NAME_None)
    {
        BlackboardComp->UnregisterObserver(IsInvestigatingKey.GetSelectedKeyID(), this);
    }

    Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}*/