#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillActor.generated.h"

UCLASS()
class ROGUEHEART_API ASkillActor : public AActor
{
    GENERATED_BODY()

public:
    ASkillActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Skill")
    virtual void ActivateSkill();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Damage = 10.0f;
};