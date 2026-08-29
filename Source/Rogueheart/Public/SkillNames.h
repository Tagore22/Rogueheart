#pragma once

#include "CoreMinimal.h"

namespace SkillNames
{
    // Player
	const FName SkillQ = TEXT("IA_Skill_Q");
	const FName SkillE = TEXT("IA_Skill_E");
	const FName SkillR = TEXT("IA_Skill_R");
    const FName BossPunchCombo = TEXT("BossPunchCombo");

    // Ohters
    const FName Shadowstep = TEXT("Shadowstep");

    // Tags
	const FName PlayerTag = TEXT("Player");
	const FName EnemyTag = TEXT("Enemy");
	const FName DieTag = TEXT("Die");
	const FName BossTag = TEXT("Boss");
    const FName ComboTag = TEXT("Combo");

    // Sockets
    const FName PlayerSocket = TEXT("Weapon_Socket1");

    // MoveSpeeds
    const float PatrolSpeed = 300.f;
    const float ChaseSpeed = 600.f;
    const float LockonSpeed = 150.f;
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