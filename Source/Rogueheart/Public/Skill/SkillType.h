#pragma once

#include "SkillType.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    None            UMETA(DisplayName = "None"),
    Fireball        UMETA(DisplayName = "Fireball"),
    IceNova         UMETA(DisplayName = "Ice Nova"),
    ThunderStrike   UMETA(DisplayName = "Thunder Strike"),
    DashSlash       UMETA(DisplayName = "Dash Slash"),
    HealingLight    UMETA(DisplayName = "Healing Light"),
};