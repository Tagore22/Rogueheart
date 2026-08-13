#pragma once

#include "CoreMinimal.h"

namespace SkillNames
{
    // Player
	const FName SkillQ = TEXT("IA_Skill_Q");
	const FName SkillE = TEXT("IA_Skill_E");
	const FName SkillR = TEXT("IA_Skill_R");

    // Ohters
    const FName Shadowstep = TEXT("Shadowstep");

    // Tags
	const FName PlayerTag = TEXT("Player");
	const FName EnemyTag = TEXT("Enemy");
	const FName DieTag = TEXT("Die");
	const FName BossTag = TEXT("Boss");
}

UENUM(BlueprintType)
enum class EPlayerState : uint8 // s
{
    Idle,
    Moving,
    Attacking,
    LockedOn,
    Dodging,
    Damaged,
    CastSkill,
    Dead
};

UENUM(BlueprintType)
enum class EActionType : uint8 // s
{
    Move,
    Attack,
    LockOn,
    Dodge,
    Parry,
    UseSkill
};