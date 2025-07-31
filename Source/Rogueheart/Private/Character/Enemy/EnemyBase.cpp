#include "Character/Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    TargetMarker = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetMarker"));
    TargetMarker->SetupAttachment(RootComponent);
    TargetMarker->SetWidgetSpace(EWidgetSpace::Screen);
    TargetMarker->SetDrawAtDesiredSize(true);
    TargetMarker->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

    static ConstructorHelpers::FClassFinder<UUserWidget> MarkerWidgetClass(TEXT("/Game/Characters/WBP_TargetMarker.WBP_TargetMarker_C"));
    if (MarkerWidgetClass.Succeeded())
    {
        TargetMarker->SetWidgetClass(MarkerWidgetClass.Class);
    }

    TargetMarker->SetVisibility(false);
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

void AEnemyBase::ShowTargetMarker(bool bShow)
{
    if (TargetMarker)
    {
        TargetMarker->SetVisibility(bShow);
    }
}