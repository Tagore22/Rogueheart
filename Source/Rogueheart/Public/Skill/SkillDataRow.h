#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillDataRow.generated.h"

class ASkillActor;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FSkillDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowAbstract = "false"))
    TSoftClassPtr<ASkillActor> SkillClass;  // 소프트 클래스

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float LifeTime;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UNiagaraSystem* Effect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Radius;
};