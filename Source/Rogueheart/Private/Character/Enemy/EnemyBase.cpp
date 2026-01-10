#include "Character/Enemy/EnemyBase.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // 타겟 마커 UI 생성
    TargetMarker = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetMarker"));
    TargetMarker->SetupAttachment(RootComponent);
    TargetMarker->SetWidgetSpace(EWidgetSpace::Screen);
    TargetMarker->SetDrawAtDesiredSize(true);
    TargetMarker->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

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
}

void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 공격 쿨다운만 업데이트
    TimeSinceLastAttack += DeltaTime;
}

void AEnemyBase::TryAttack()
{
    if (CanAttack() && AttackMontage)
    {
        if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
        {
            if (!AnimInstance->Montage_IsPlaying(AttackMontage))
            {
                AnimInstance->Montage_Play(AttackMontage);
                ResetAttackCooldown();
                UE_LOG(LogTemp, Warning, TEXT("Attack Executed"));
            }
        }
    }
}

bool AEnemyBase::CanAttack() const
{
    return TimeSinceLastAttack >= AttackCooldown;
}

void AEnemyBase::ResetAttackCooldown()
{
    TimeSinceLastAttack = 0.f;
}

void AEnemyBase::ShowTargetMarker(bool bShow)
{
    if (IsValid(TargetMarker))
    {
        TargetMarker->SetVisibility(bShow);
    }
}