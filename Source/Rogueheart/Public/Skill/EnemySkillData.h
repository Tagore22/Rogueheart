#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "EnemySkillData.generated.h"

USTRUCT(BlueprintType)
struct FEnemySkillData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    FName SkillName;

    UPROPERTY(EditDefaultsOnly)
    FName SkillID;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ASkillBase> Type;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class AActor> SpawnActor;

    UPROPERTY(EditDefaultsOnly)
    class UAnimMontage* SkillMontage;

    UPROPERTY(EditDefaultsOnly)
    TArray<float> Damage;

    UPROPERTY(EditDefaultsOnly)
    TArray<float> Cooldown;

    UPROPERTY(EditDefaultsOnly)
    TArray<float> SpawnTime;

    UPROPERTY(EditDefaultsOnly)
    class UTexture2D* SkillIcon;

    UPROPERTY(EditDefaultsOnly)
    int32 MaxRepeatNum;

    UPROPERTY(EditDefaultsOnly)
    class UNiagaraSystem* Effect;

    UPROPERTY(EditDefaultsOnly)
    TArray<float> Cost;

    UPROPERTY(EditDefaultsOnly)
    TArray<float> SpeedRatio;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UMaterialInterface> Material;
};
