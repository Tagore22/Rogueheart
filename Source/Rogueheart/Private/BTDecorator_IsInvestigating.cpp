#include "BTDecorator_IsInvestigating.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInvestigating::UBTDecorator_IsInvestigating()
{
    NodeName = TEXT("Is Investigating");

    // Blackboard Key 필터 설정
    IsInvestigatingKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInvestigating, IsInvestigatingKey));

    // UE 5.1 권장 Observer 설정
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = true;
    bNotifyTick = false;

    // FlowAbortMode 설정 - 키 변경 시 재평가를 위해
    FlowAbortMode = EBTFlowAbortMode::Self;
}

bool UBTDecorator_IsInvestigating::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // 블랙보드 컴포넌트 유효성 체크
    const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    // 키가 설정되어 있는지 체크
    if (IsInvestigatingKey.SelectedKeyName == NAME_None)
    {
        return false;
    }

    // 블랙보드에서 Boolean 값 가져오기
    return BlackboardComp->GetValueAsBool(IsInvestigatingKey.SelectedKeyName);
}

FString UBTDecorator_IsInvestigating::GetStaticDescription() const
{
    return FString::Printf(TEXT("Is Investigating: %s"),
        IsInvestigatingKey.SelectedKeyName != NAME_None ?
        *IsInvestigatingKey.SelectedKeyName.ToString() : TEXT("None"));
}