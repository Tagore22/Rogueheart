#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdatePlayerState.generated.h"

UCLASS()
class ROGUEHEART_API UBTService_UpdatePlayerState : public UBTService_BlackboardBase
{
    GENERATED_BODY()

public:
    UBTService_UpdatePlayerState();

protected:
    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "AI")
    float TargetRangeDistance = 500.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRangeDistance = 200.f;
    
    //
    UPROPERTY(EditAnywhere, Category = "AI")
    float MaxChaseDistance = 1000.f;  // ★ 이 줄 추가
    //
};