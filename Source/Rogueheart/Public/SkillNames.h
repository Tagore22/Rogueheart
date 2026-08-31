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

    // BlackBoard Values;
    const FName TargetPlayer = TEXT("TargetPlayer");
    const FName SpawnLocation = TEXT("SpawnLocation");
    const FName LastPerceptionTime = TEXT("LastPerceptionTime"); // 가장 최근에 적이 플레이어를 감지한 시간.
    const FName MaxDistance = TEXT("MaxDistance");               // 스폰위치로부터 멀어질 수 있는 최대 거리.
    const FName LostTargetTime = TEXT("LostTargetTime");   // 적이 플레이어를 감지해야하는 최대 시간.
    const FName bIsPerception = TEXT("bIsPerception"); // 적이 플레이어를 탐지중인가?
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