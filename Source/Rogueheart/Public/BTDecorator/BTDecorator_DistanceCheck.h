#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_DistanceCheck.generated.h"

UENUM(BlueprintType)
enum class EDistanceCheckType : uint8
{
    Distance2D,
    Distance3D
};

UCLASS()
class ROGUEHEART_API UBTDecorator_DistanceCheck : public UBTDecorator
{
    GENERATED_BODY()

public:
    UBTDecorator_DistanceCheck();

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector FirstLocationKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector SecondLocationKey;

    UPROPERTY(EditAnywhere, Category = "Distance", meta = (ClampMin = "0.0"))
    float MinDistance = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Distance", meta = (ClampMin = "0.0"))
    float MaxDistance = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Distance")
    EDistanceCheckType DistanceType = EDistanceCheckType::Distance2D;

    UPROPERTY(EditAnywhere, Category = "Distance")
    bool bInvertCondition = false;

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
    virtual FString GetStaticDescription() const override;

private:
    float CalculateDistance(const FVector& LocationA, const FVector& LocationB) const;
};