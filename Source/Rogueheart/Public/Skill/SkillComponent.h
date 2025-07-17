#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Skill/SkillDataRow.h"
#include "SkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillCooldownUpdated, FName, SkillName, float, RemainingTime);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUEHEART_API USkillComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USkillComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    /** DataTable에서 스킬 속성 불러올 에셋 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills")
        UDataTable* SkillDataTable;

    /** 스킬 발동 함수 */
    UFUNCTION(BlueprintCallable, Category = "Skills")
        void UseSkill(const FName& SkillRowName);

    /** SkillName, RemainingCooldownSeconds 브로드캐스트 */
    UPROPERTY(BlueprintAssignable, Category = "Skills")
        FOnSkillCooldownUpdated OnSkillCooldownUpdated;

private:
    /** 쿨다운 타이머 관리 */
    TMap<FName, float> CooldownTimers;

    void UpdateCooldowns(float DeltaTime);
};