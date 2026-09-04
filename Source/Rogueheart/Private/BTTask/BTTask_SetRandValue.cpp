#include "BTTask/BTTask_SetRandValue.h"
#include "SkillNames.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SetRandValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // BB에서 bIsPhaseTwo를 가져와 RandRange의 범위값을 알아낸다.
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

    bool bIsPhaseTwo = BB->GetValueAsBool(SkillNames::bIsPhaseTwo); 
    int32 MaxScope = bIsPhaseTwo ? SkillNames::PhaseTwoMax : SkillNames::PhaseOneMax;
    int32 RandValue = FMath::RandRange(SkillNames::PhaseOneMin, MaxScope);
    UE_LOG(LogTemp, Warning, TEXT("RandValue : %d"), RandValue);

    // BB에 랜덤값을 저장한다.
    BB->SetValueAsInt(SkillNames::RandValue, RandValue);
    return EBTNodeResult::Succeeded;
}

