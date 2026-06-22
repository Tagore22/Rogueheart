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
    float Damage;

    UPROPERTY(EditDefaultsOnly)
    float Cooldown;

    UPROPERTY(EditDefaultsOnly)
    class UTexture2D* SkillIcon;
};
