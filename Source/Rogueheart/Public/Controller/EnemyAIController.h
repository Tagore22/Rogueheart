#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

UCLASS()
class ROGUEHEART_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UAIPerceptionComponent* AIPerception;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UAISenseConfig_Sight* SightConfig;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackDistance = 200.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    class UAnimMontage* AttackMontage;

    UPROPERTY()
    AActor* TargetActor;
};
