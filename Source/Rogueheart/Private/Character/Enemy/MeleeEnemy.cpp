#include "Character/Enemy/MeleeEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"

AMeleeEnemy::AMeleeEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMeleeEnemy::BeginPlay()
{
    Super::BeginPlay();

    SetEnemyState(EMeleeEnemyState::Approaching);
}

void AMeleeEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeSinceLastAction += DeltaTime;

    UpdateState(DeltaTime);
}

void AMeleeEnemy::UpdateState(float DeltaTime)
{
    if (!TargetPlayer)
        return;

    float DistanceToPlayer = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

    switch (CurrentState)
    {
    case EMeleeEnemyState::Approaching:
    {
        if (DistanceToPlayer <= CircleRadius + 50.f)
        {
            SetEnemyState(EMeleeEnemyState::Circling);
        }
        else
        {
            MoveToPlayer();
        }
        break;
    }

    case EMeleeEnemyState::Circling:
    {
        if (DistanceToPlayer > CircleRadius + 100.f)
        {
            SetEnemyState(EMeleeEnemyState::Approaching);
        }
        else if (ShouldAttack())
        {
            SetEnemyState(EMeleeEnemyState::Attacking);
        }
        else
        {
            MoveAroundPlayer();
        }
        break;
    }

    case EMeleeEnemyState::Attacking:
    {
        PerformMeleeAttack();
        SetEnemyState(EMeleeEnemyState::Cooldown);
        break;
    }

    case EMeleeEnemyState::Cooldown:
    {
        if (TimeSinceLastAction >= AttackInterval)
        {
            SetEnemyState(EMeleeEnemyState::Circling);
        }
        break;
    }

    default:
        break;
    }
}

void AMeleeEnemy::SetEnemyState(EMeleeEnemyState NewState)
{
    CurrentState = NewState;
    TimeSinceLastAction = 0.f;
}

bool AMeleeEnemy::ShouldAttack() const
{
    return TimeSinceLastAction >= AttackInterval;
}

void AMeleeEnemy::PerformMeleeAttack()
{
    if (AttackMontage)
    {
        PlayAnimMontage(AttackMontage);
    }
}

void AMeleeEnemy::MoveAroundPlayer()
{
    if (!TargetPlayer)
        return;

    UE_LOG(LogTemp, Warning, TEXT("MoveAround"));
    FVector PlayerLocation = TargetPlayer->GetActorLocation();
    FVector ToEnemy = GetActorLocation() - PlayerLocation;
    ToEnemy.Z = 0.f;
    ToEnemy.Normalize();

    FVector RightDir = FVector::CrossProduct(FVector::UpVector, ToEnemy);
    FVector CirclePos = PlayerLocation + ToEnemy * CircleRadius + RightDir * CircleSpeed * GetWorld()->GetDeltaSeconds();

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSys)
    {
        FNavLocation NavLocation;
        if (NavSys->ProjectPointToNavigation(CirclePos, NavLocation))
        {
            AAIController* AICon = Cast<AAIController>(GetController());
            if (AICon)
            {
                AICon->MoveToLocation(NavLocation.Location);
            }
        }
    }

    // 디버그용 회전 궤적 표시
    // DrawDebugCircle(GetWorld(), PlayerLocation, CircleRadius, 32, FColor::Red, false, 0.1f, 0, 2.f, FVector(1,0,0), FVector(0,1,0), false);
}