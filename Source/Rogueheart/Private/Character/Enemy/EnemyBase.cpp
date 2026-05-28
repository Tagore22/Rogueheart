#include "Character/Enemy/EnemyBase.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
//#include "Components/CapsuleComponent.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
    // 일단은 중요한 일들이 자식클래스에 있을 확률이 높아 이곳은 비워둔다.
    // 이후에 공통적으로 묶어야할 일이 있거든 이곳에 구현하되, 없다면 순수 가상 함수도
    // 생각해봄직하다.
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

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage >= CurHP)
    {
        EnemyDie();
    }
    else
    {
        // 현재 체력이 ActualDamage만큼 줄어든다.
        // 만약 체력이 0보다 작다면 사망.
        // 피격 애니메이션 실행.
        UAnimInstance* Anim = GetMesh()->GetAnimInstance();
        if (!Anim || DamagedMontages.Num() == 0)
            return ActualDamage;

        UE_LOG(LogTemp, Warning, TEXT("Enemy Take %f Damage!"), ActualDamage);

        //
        int32 DamagedIndex = FMath::RandRange(1, DamagedMontages.Num() - 1);
        Anim->Montage_Play(DamagedMontages[DamagedIndex]);
        CurHP -= ActualDamage;
    }

    return ActualDamage;
}

void AEnemyBase::EnemyDie()
{
    UAnimInstance* Anim = GetMesh()->GetAnimInstance();
    if (!Anim || !AMT_Die)
        return;

    Anim->Montage_Play(AMT_Die);
    // 꼭 콜리전을 꺼야 하는가?
    FTimerHandle DestroyTimer;
    GetWorldTimerManager().SetTimer(DestroyTimer, [this]()
        {
            Destroy();
        }, DyingTime, false);
}