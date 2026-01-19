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
    if (!CanAttack() || !IsValid(AttackMontage))
        return;

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance)
        return;

    if (AnimInstance->Montage_IsPlaying(AttackMontage))
        return;

    // 혹여나 애니메이션이 제대로 재생되지 않았을 경우를 처리하기 위한 임시 방편으로 남겨둔다.
    /*const float PlayLength = AnimInstance->Montage_Play(AttackMontage);
    if (PlayLength > 0.f)
    {
        ResetAttackCooldown();
        UE_LOG(LogTemp, Warning, TEXT("Attack Executed"));
    }
    else
    {
    }*/
    ResetAttackCooldown();
    UE_LOG(LogTemp, Warning, TEXT("Attack Executed"));
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