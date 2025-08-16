#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasValidPatrolPoint.generated.h"

UCLASS()
class ROGUEHEART_API UBTDecorator_HasValidPatrolPoint : public UBTDecorator
{
    GENERATED_BODY()

public:
    UBTDecorator_HasValidPatrolPoint();

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
    virtual FString GetStaticDescription() const override;
};