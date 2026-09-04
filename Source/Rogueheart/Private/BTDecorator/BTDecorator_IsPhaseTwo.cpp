#include "BTDecorator/BTDecorator_IsPhaseTwo.h"
#include "SkillNames.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_IsPhaseTwo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

    int32 RandValue = BB->GetValueAsInt(SkillNames::RandValue);

    return RandValue >= SkillNames::PhaseTwoMin;
}
