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

    // 고유 ID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
    FName SkillID;

    // 스킬 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        FText SkillName;

    // 스킬 타입 (공격/방어/유틸)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        ESkillType SkillType;

    // 쿨다운 시간 (초)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        float Cooldown;

    // 소모 자원 (예: MP)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        float ResourceCost;

    // 애니메이션 몽타주
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        UAnimMontage* Montage;

    // 이펙트 (파티클)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        UParticleSystem* Effect;

    // 사운드
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
        USoundBase* Sound;

    // (옵션) 최대 레벨 & 레벨별 파라미터
    // 예: 레벨에 따라 데미지, 범위 등이 달라질 때 사용
    // TMap<int32, FSkillLevelData> LevelData;
};