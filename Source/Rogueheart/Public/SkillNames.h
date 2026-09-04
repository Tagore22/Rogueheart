#pragma once

#include "CoreMinimal.h"

namespace SkillNames
{
    // SkillNames
	const FName SkillQ = TEXT("IA_Skill_Q");
	const FName SkillE = TEXT("IA_Skill_E");
	const FName SkillR = TEXT("IA_Skill_R");
    const FName BossPunchCombo = TEXT("BossPunchCombo");

    // Tags
	const FName PlayerTag = TEXT("Player");
	const FName EnemyTag = TEXT("Enemy");
	const FName DieTag = TEXT("Die");
	const FName BossTag = TEXT("Boss");
    const FName ComboTag = TEXT("Combo");

    // Sockets
    const FName PlayerSocket = TEXT("Weapon_Socket1");

    // MoveSpeeds
    const float LockonSpeed = 50.f;
    const float PatrolSpeed = 300.f;
    const float ChaseSpeed = 450.f;

    // BlackBoard Values
    const FName TargetPlayer = TEXT("TargetPlayer");
    const FName SpawnLocation = TEXT("SpawnLocation");
    const FName LastPerceptionTime = TEXT("LastPerceptionTime"); // 가장 최근에 적이 플레이어를 감지한 시간.
    const FName MaxDistance = TEXT("MaxDistance");               // 스폰위치로부터 멀어질 수 있는 최대 거리.
    const FName LostTargetTime = TEXT("LostTargetTime");   // 적이 플레이어를 감지해야하는 최대 시간.
    const FName bIsPerception = TEXT("bIsPerception"); // 적이 플레이어를 탐지중인가?
    const FName MaxLockonDist = TEXT("MaxLockonDist"); // 락온 거리.
    const FName MaxAttackDist = TEXT("MaxAttackDist"); // 공격 거리.
    const FName CircleLocation = TEXT("CircleLocation"); // 락온 상태일 때 다음 위치.
    const FName UnlimitedDist = TEXT("UnlimitedDist"); // 락온 상태일 때 다음 위치.
    const FName ZeroDist = TEXT("ZeroDist"); // 락온 상태일 때 다음 위치.
    const FName bIsPhaseTwo = TEXT("bIsPhaseTwo"); // 현재 페이즈2에 돌입하였는가.
    const FName RandValue = TEXT("RandValue"); // 행동양식에 분기점을 나눌 랜덤값.

    // PhaseValue Scope
    const int32 PhaseOneMin = 0;
    const int32 PhaseOneMax = 7;
    const int32 PhaseTwoMin = 8;
    const int32 PhaseTwoMax = 10;
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