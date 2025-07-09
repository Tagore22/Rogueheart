#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class ROGUEHEART_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()
};//

/*public:
    AEnemyAIController();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
        UAIPerceptionComponent* AIPerception;

    UPROPERTY()
        UAISenseConfig_Sight* SightConfig;

    UFUNCTION()
        void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};*/