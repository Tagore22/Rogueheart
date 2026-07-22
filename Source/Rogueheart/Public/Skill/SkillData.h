#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "SkillData.generated.h"

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
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
    float Damage;

    UPROPERTY(EditDefaultsOnly)
    float Cooldown;

    UPROPERTY(EditDefaultsOnly)
    float SpawnTime;

    UPROPERTY(EditDefaultsOnly)
    class UTexture2D* SkillIcon;

    UPROPERTY(EditDefaultsOnly)
    int32 MaxRepeatNum;
};
