#include "Character/Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();
    TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TargetPlayer) return;

    float Distance = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

    if (Distance <= DetectRange)
    {
        if (Distance > AttackRange)
        {
            MoveToPlayer();
        }
        else
        {
            TryAttack();
        }
    }

    TimeSinceLastAttack += DeltaTime;
}

void AEnemyBase::MoveToPlayer()
{
    AAIController* AICon = Cast<AAIController>(GetController());
    if (AICon && TargetPlayer)
    {
        AICon->MoveToActor(TargetPlayer, 5.f);
    }
}

void AEnemyBase::TryAttack()
{
    if (TimeSinceLastAttack >= AttackCooldown)
    {
        PlayAnimMontage(AttackMontage);
        TimeSinceLastAttack = 0.f;
    }
}