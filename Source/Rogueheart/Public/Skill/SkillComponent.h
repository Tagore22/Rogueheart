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
    /** DataTable���� ��ų �Ӽ� �ҷ��� ���� */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills")
        UDataTable* SkillDataTable;

    /** �� ��ų�� ���� ���� ���� */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
        TMap<FName, int32> SkillLevels;

    /** ��ų �ߵ� �Լ� */
    UFUNCTION(BlueprintCallable, Category = "Skills")
        void UseSkill(ESkillType SkillType);

    /** ��ų ��ȭ �Լ� */
    UFUNCTION(BlueprintCallable, Category = "Skills")
        bool UpgradeSkill(ESkillType SkillType);

    /** ���� ��ų �������� �������� */
    UFUNCTION(BlueprintCallable, Category = "Skills")
        float GetSkillDamage(ESkillType SkillType) const;

    /** SkillType, RemainingCooldownSeconds ��ε�ĳ��Ʈ */
    UPROPERTY(BlueprintAssignable, Category = "Skills")
        FOnSkillCooldownUpdated OnSkillCooldownUpdated;

private:
    /** SkillType �� ���� ��ٿ� �� */
    TMap<ESkillType, float> CooldownTimers;

    /** SkillType �� DataTable RowName ���� */
    TMap<ESkillType, FName> SkillRowNames;

    void UpdateCooldowns(float DeltaTime);
    const FSkillDataRow* GetSkillData(ESkillType SkillType) const;
    void SpawnSkillActor(const FSkillDataRow& SkillData, float Damage);
};