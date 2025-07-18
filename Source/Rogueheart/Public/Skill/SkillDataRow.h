#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillType.h"   
#include "SkillDataRow.generated.h"

class ASkillActor;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ESkillType SkillType;

    // SoftClassPtr 방식으로 변경
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowAbstract = "false"))
    TSoftClassPtr<ASkillActor> SkillClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LifeTime;

    // 이펙트 필드
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UNiagaraSystem* Effect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* SkillIcon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Range;
};