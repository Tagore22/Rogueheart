#pragma once

#include "CoreMinimal.h"
#include "SkillData.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8 {
    Offensive,
    Defensive,
    Utility
};

USTRUCT(BlueprintType)
struct FSkillData 
{
    GENERATED_BODY()

    // ���� ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
    FName SkillID;

    // ��ų �̸�
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        FText SkillName;

    // ��ų Ÿ�� (����/���/��ƿ)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        ESkillType SkillType;

    // ��ٿ� �ð� (��)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        float Cooldown;

    // �Ҹ� �ڿ� (��: MP)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        float ResourceCost;

    // �ִϸ��̼� ��Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        UAnimMontage* Montage;

    // ����Ʈ (��ƼŬ)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        UParticleSystem* Effect;

    // ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        USoundBase* Sound;

    // (�ɼ�) �ִ� ���� & ������ �Ķ����
    // ��: ������ ���� ������, ���� ���� �޶��� �� ���
    // TMap<int32, FSkillLevelData> LevelData;
};