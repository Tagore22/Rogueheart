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
    /** DataTable���� ��ų �Ӽ� �ҷ��� ���� */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skills")
        UDataTable* SkillDataTable;

    /** ��ų �ߵ� �Լ� */
    UFUNCTION(BlueprintCallable, Category = "Skills")
        void UseSkill(const FName& SkillRowName);

    /** SkillName, RemainingCooldownSeconds ��ε�ĳ��Ʈ */
    UPROPERTY(BlueprintAssignable, Category = "Skills")
        FOnSkillCooldownUpdated OnSkillCooldownUpdated;

private:
    /** ��ٿ� Ÿ�̸� ���� */
    TMap<FName, float> CooldownTimers;

    void UpdateCooldowns(float DeltaTime);
};