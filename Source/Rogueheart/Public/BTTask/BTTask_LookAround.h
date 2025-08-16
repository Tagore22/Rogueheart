#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LookAround.generated.h"

UCLASS()
class ROGUEHEART_API UBTTask_LookAround : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_LookAround();

    // 둘러보기 지속 시간
    UPROPERTY(EditAnywhere, Category = "LookAround", meta = (ClampMin = "0.5", ClampMax = "10.0"))
    float LookAroundDuration = 2.0f;

    // 회전 각도 (좌우로 얼마나 돌지)
    UPROPERTY(EditAnywhere, Category = "LookAround", meta = (ClampMin = "30.0", ClampMax = "180.0"))
    float LookAroundAngle = 90.0f;

    // 회전 속도
    UPROPERTY(EditAnywhere, Category = "LookAround", meta = (ClampMin = "30.0", ClampMax = "360.0"))
    float RotationSpeed = 180.0f;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    virtual FString GetStaticDescription() const override;

private:
    struct FLookAroundMemory
    {
        float ElapsedTime;
        FRotator InitialRotation;
        FRotator TargetRotation;
        bool bTurningRight;
        bool bReturningToCenter;

        FLookAroundMemory()
        {
            Reset();
        }

        void Reset()
        {
            ElapsedTime = 0.0f;
            InitialRotation = FRotator::ZeroRotator;
            TargetRotation = FRotator::ZeroRotator;
            bTurningRight = true;
            bReturningToCenter = false;
        }
    };
};
