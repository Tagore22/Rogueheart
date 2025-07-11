#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/SkillActor.h"  
#include "SkillComponent.generated.h"

USTRUCT(BlueprintType)
struct FSkillData
{
    GENERATED_BODY()


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cooldown = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ASkillActor> SkillClass;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUEHEART_API USkillComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USkillComponent();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
    TMap<FName, FSkillData> Skills;

    UPROPERTY()
    TMap<FName, float> CooldownTimers;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void UseSkill(FName SkillName);

private:
    void UpdateCooldowns(float DeltaTime);
};