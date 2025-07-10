#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillData.h"
#include "SkillComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUEHEART_API USkillComponent : public UActorComponent {
    GENERATED_BODY()

public:
    USkillComponent();

    // 스킬 배열 (DataTable 또는 직접 세팅)
    UPROPERTY(EditAnywhere, Category = "Skills")
        TArray<FSkillData> Skills;

    // 내부 쿨다운 타이머 관리용
    TMap<FName, float> CooldownTimers;

    // 스킬 발동
    UFUNCTION(BlueprintCallable)
        bool UseSkill(FName SkillID);

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // 내부: 쿨다운 업데이트
    void UpdateCooldowns(float DeltaTime);

    // 내부: 실제 스킬 실행 로직
    void ExecuteSkill(const FSkillData& Data);
};