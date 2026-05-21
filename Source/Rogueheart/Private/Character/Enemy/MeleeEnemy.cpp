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

void AMeleeEnemy::TryAttack()
{
    UE_LOG(LogTemp, Warning, TEXT("Attack Start!"));
    Super::TryAttack();

    UAnimInstance* Anim = GetMesh()->GetAnimInstance();
    if (!Anim || !AMT_Attack)
        return;

    UE_LOG(LogTemp, Warning, TEXT("Rand Number"));
    int32 CurAttackNum = FMath::RandRange(1, 4);
    UE_LOG(LogTemp, Warning, TEXT("Animation Start"));
    Anim->Montage_Play(AMT_Attack);
    Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("Attack%d"), CurAttackNum)), AMT_Attack);
    // 여기서 공격하는 애니메이션을 실행한다.
    // Attack 몽타주를 실행하되, 1 ~ N까지의 랜덤한 값을 뽑아 점프하여 실행한다.
    // Sweep 제대로 되나 확인할 것.
    UE_LOG(LogTemp, Warning, TEXT("Enemy Attack!"));
}