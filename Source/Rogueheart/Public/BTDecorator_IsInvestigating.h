#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInvestigating.generated.h"

UCLASS()
class ROGUEHEART_API UBTDecorator_IsInvestigating : public UBTDecorator
{
    GENERATED_BODY()
};

/*public:
    UBTDecorator_IsInvestigating();

    UPROPERTY(EditAnywhere, Category = "Blackboard")
        FBlackboardKeySelector IsInvestigatingKey;

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
    virtual FString GetStaticDescription() const override;
    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};*/