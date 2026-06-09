#include "Character/Enemy/EnemyBase.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/EnemyHPBarWidget.h"
#include "WeaponSweepComponent.h"


AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));
    TargetWidget->SetupAttachment(RootComponent);
    TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);
    TargetWidget->SetVisibility(false);
    TargetWidget->SetRelativeLocation(FVector(0.f, 0.f, -20.f)); // 후에 에디터에서 수정 이후 확정지을 것.

    HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
    HPBarWidget->SetupAttachment(RootComponent);
    HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
    HPBarWidget->SetVisibility(false);
    HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 45.f)); // 후에 에디터에서 수정 이후 확정지을 것.

    SweepCom = CreateDefaultSubobject<UWeaponSweepComponent>(TEXT("SweepComponent"));
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

    if (bIsTargeted)
        return;

    if (HPBarTimer < 0.f)
    {
        return;
    }
    else if (HPBarTimer < HPBarEndTime)
    {
        HPBarTimer += DeltaTime;
    }
    else if (HPBarTimer >= HPBarEndTime)
    {
        HPBarTimer = -1.f;
        ShowHPBarWidget(false);
    }
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

void AEnemyBase::ShowTargetWidget(bool bShow)
{
    // 어떠한 적이 타겟팅 되었을 때에는 반드시 체력바도 등장하게 되어있다.
    // 그 반대는 성립되지 않는다.

    if (TargetWidget)
    {
        TargetWidget->SetVisibility(bShow);
    }
}

void AEnemyBase::ShowHPBarWidget(bool bShow)
{
    if (HPBarWidget)
    {
        HPBarWidget->SetVisibility(bShow);
    }
    ResetHPBarTimer();
}

void AEnemyBase::ResetHPBarTimer()
{
    HPBarTimer = 0.f;
}

void AEnemyBase::SetIsTargeted(bool bTargeted)
{
    if (bTargeted)
    {
        bIsTargeted = true;
        HPBarTimer = -1.f;
    }
    else
    {
        bIsTargeted = false;
    }
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (CurHP <= 0.f)
        return ActualDamage;

    ShowHPBarWidget(true);
    CurHP = FMath::Max(CurHP - ActualDamage, 0.f);
    UEnemyHPBarWidget* HPBar = Cast<UEnemyHPBarWidget>(HPBarWidget->GetUserWidgetObject());
    if (!HPBar)
        return ActualDamage;
    HPBar->SetHPPercent(CurHP / MaxHP);

    if (CurHP <= 0.f)
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
        int32 DamagedIndex = FMath::RandRange(0, DamagedMontages.Num() - 1);
        Anim->Montage_Play(DamagedMontages[DamagedIndex]);
    }

    return ActualDamage;
}

void AEnemyBase::EnemyDie()
{
    UAnimInstance* Anim = GetMesh()->GetAnimInstance();
    if (!Anim || !AMT_Die)
        return;

    Anim->Montage_Play(AMT_Die);

    AAIController* AIC = Cast<AAIController>(GetController());
    if (!AIC)
        return;

    AIC->StopMovement();
    AIC->BrainComponent->StopLogic(TEXT("EnemyDie"));

    ShowTargetWidget(false);

    // 꼭 콜리전을 꺼야 하는가?
    FTimerHandle DestroyTimer;
    GetWorldTimerManager().SetTimer(DestroyTimer, [this]()
        {
            Destroy();
        }, DyingTime, false);
}

float AEnemyBase::GetCurHP() const
{
    return CurHP;
}

float AEnemyBase::GetMaxHP() const
{
    return MaxHP;
}