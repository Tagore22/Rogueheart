#include "Character/Enemy/MeleeEnemy.h"

AMeleeEnemy::AMeleeEnemy()
{
    // ���� ���� ���� �⺻�� ����
    AttackRange = 150.f;
    AttackCooldown = 2.f;
    CircleRadius = 250.f;
    CircleSpeed = 200.f;
    AttackInterval = 3.f;
}