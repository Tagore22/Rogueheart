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

    // ��ų �迭 (DataTable �Ǵ� ���� ����)
    UPROPERTY(EditAnywhere, Category = "Skills")
        TArray<FSkillData> Skills;

    // ���� ��ٿ� Ÿ�̸� ������
    TMap<FName, float> CooldownTimers;

    // ��ų �ߵ�
    UFUNCTION(BlueprintCallable)
        bool UseSkill(FName SkillID);

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // ����: ��ٿ� ������Ʈ
    void UpdateCooldowns(float DeltaTime);

    // ����: ���� ��ų ���� ����
    void ExecuteSkill(const FSkillData& Data);
};