#include "Character/Enemy/MeleeEnemy.h"

AMeleeEnemy::AMeleeEnemy()
{
    // 근접 전투 전용 기본값 설정
    AttackRange = 150.f;
    AttackCooldown = 2.f;
    CircleRadius = 250.f;
    CircleSpeed = 200.f;
    AttackInterval = 3.f;
}