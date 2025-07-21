#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Skill/SkillType.h"        
#include "Skill/SkillDataRow.h"
#include "SkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillCooldownUpdated, ESkillType, SkillType, float, RemainingTime);

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

    /** 각 스킬별 현재 레벨 저장 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
        TMap<FName, int32> SkillLevels;

    /** 스킬 발동 함수 */
    UFUNCTION(BlueprintCallable, Category = "Skills")
        void UseSkill(ESkillType SkillType);

    /** 스킬 강화 함수 */
    UFUNCTION(BlueprintCallable, Category = "Skills")
        bool UpgradeSkill(ESkillType SkillType);

    /** 현재 스킬 데미지를 가져오기 */
    UFUNCTION(BlueprintCallable, Category = "Skills")
        float GetSkillDamage(ESkillType SkillType) const;

    /** SkillType, RemainingCooldownSeconds 브로드캐스트 */
    UPROPERTY(BlueprintAssignable, Category = "Skills")
        FOnSkillCooldownUpdated OnSkillCooldownUpdated;

private:
    /** SkillType → 남은 쿨다운 초 */
    TMap<ESkillType, float> CooldownTimers;

    /** SkillType → DataTable RowName 매핑 */
    TMap<ESkillType, FName> SkillRowNames;

    void UpdateCooldowns(float DeltaTime);
    const FSkillDataRow* GetSkillData(ESkillType SkillType) const;
    void SpawnSkillActor(const FSkillDataRow& SkillData, float Damage);
};