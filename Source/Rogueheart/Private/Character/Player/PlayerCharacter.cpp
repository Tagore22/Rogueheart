#include "Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Player/PlayerAnimInstance.h"
#include "Skill/SkillCooldownWidget.h"
#include "Character/Enemy/EnemyBase.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

    SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    SetGenericTeamId(FGenericTeamId(TeamID));
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
        EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
        EnhancedInput->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APlayerCharacter::Dodge);
        EnhancedInput->BindAction(IA_Skill1, ETriggerEvent::Started, this, &APlayerCharacter::UseFireball);
        EnhancedInput->BindAction(IA_Skill2, ETriggerEvent::Started, this, &APlayerCharacter::UseIceBlast);
        EnhancedInput->BindAction(IA_LockOn, ETriggerEvent::Started, this, &APlayerCharacter::ToggleLockOn);
        EnhancedInput->BindAction(IA_SwitchTargetLeft, ETriggerEvent::Started, this, &APlayerCharacter::SwitchTargetLeft);
        EnhancedInput->BindAction(IA_SwitchTargetRight, ETriggerEvent::Started, this, &APlayerCharacter::SwitchTargetRight);
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsLockedOn && LockOnTarget)
    {
        UpdateLockOnRotation(DeltaTime);
        CheckLockOnDistance();
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector2D = Value.Get<FVector2D>();
    LastMoveInput = FVector(MovementVector2D.X, MovementVector2D.Y, 0.f);

    if (!CanAct() || MovementVector2D.IsNearlyZero())
        return;

    if (Controller)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector2D.X);
        AddMovementInput(RightDirection, MovementVector2D.Y);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    if (bIsLockedOn)
        return;

    const FVector2D LookAxis = Value.Get<FVector2D>();

    if (LookAxis.IsNearlyZero())
        return;

    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
    if (CurrentState == EPlayerState::Dodging || CurrentState == EPlayerState::Stunned)
        return;

    if (CurrentState != EPlayerState::Attacking)
    {
        CurrentCombo = 1;
        bInputCombo = false;
        // 공격 애니메이션 도중 NextCombo 노티파이에 의해서만 true로 바뀜.
        bCanNextCombo = false; 

        SetPlayerState(EPlayerState::Attacking);
        PlayComboMontage();
    }
    else if (bCanNextCombo && !bInputCombo)
    {
        bInputCombo = true;
    }
} 

void APlayerCharacter::PlayComboMontage()
{
    UPlayerAnimInstance* Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
    if (Anim && AMT_Attack)
    {
        Anim->Montage_Play(AMT_Attack);
        Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("Combo%d"), CurrentCombo)), AMT_Attack);
    }
}

void APlayerCharacter::HandleComboInput()
{
    ++CurrentCombo;
    bInputCombo = false;
    bCanNextCombo = false;

    PlayComboMontage();
}

void APlayerCharacter::OnAttackEnd()
{
    if (bInputCombo && CurrentCombo < MaxCombo)
    {
        HandleComboInput();
    }
    else
    {
        CurrentCombo = 0;
        bInputCombo = false;
        bCanNextCombo = false;
        SetPlayerState(EPlayerState::Idle);
    }
}

// 여기부터.
void APlayerCharacter::RestoreLockOnIfNeeded()
{
    if (bWasLockedOnWhenDodged)
    {
        FindNearestTarget();

        if (LockOnTarget)
        {
            bIsLockedOn = true;
            GetCharacterMovement()->bOrientRotationToMovement = false;
            bUseControllerRotationYaw = true;
        }

        bWasLockedOnWhenDodged = false;
    }
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
    if (!CanAct() || AMT_Dodge == nullptr)
        return;

    SetPlayerState(EPlayerState::Dodging);
    bWasLockedOnWhenDodged = bIsLockedOn;

    if (!LastMoveInput.IsNearlyZero())
    {
        FRotator ControlRot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
        FVector DodgeDir = ControlRot.RotateVector(LastMoveInput);
        SetActorRotation(DodgeDir.Rotation());
    }

    if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
    {
        Anim->Montage_Play(AMT_Dodge);
    }

    if (bIsLockedOn)
    {
        bIsLockedOn = false;
        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
    }
}

void APlayerCharacter::UseFireball()
{
    if (!CanAct()) return;
    SkillComponent->UseSkill(ESkillType::Fireball);
}

void APlayerCharacter::UseIceBlast()
{
    if (!CanAct()) return;
    SkillComponent->UseSkill(ESkillType::IceNova);
}

void APlayerCharacter::SetPlayerState(EPlayerState NewState)
{
    CurrentState = NewState;
}

bool APlayerCharacter::CanAct() const
{
    return CurrentState != EPlayerState::Attacking && CurrentState != EPlayerState::Dodging && CurrentState != EPlayerState::Stunned;
}

void APlayerCharacter::ToggleLockOn()
{
    if (!CanAct())
        return;

    if (bIsLockedOn)
    {
        if (CurrentTargetEnemy)
        {
            CurrentTargetEnemy->ShowTargetMarker(false);
        }

        bIsLockedOn = false;
        LockOnTarget = nullptr;
        CurrentTargetEnemy = nullptr;

        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
    }
    else
    {
        FindNearestTarget();
        CurrentTargetEnemy = Cast<AEnemyBase>(LockOnTarget);

        if (CurrentTargetEnemy)
        {
            bIsLockedOn = true;
            CurrentTargetEnemy->ShowTargetMarker(true);

            GetCharacterMovement()->bOrientRotationToMovement = false;
            bUseControllerRotationYaw = true;
        }
    }
}

void APlayerCharacter::FindNearestTarget()
{
    float NearestDist = LockOnRange;
    AActor* NearestEnemy = nullptr;

    for (TActorIterator<AEnemyBase> It(GetWorld()); It; ++It)
    {
        AEnemyBase* Enemy = *It;
        if (!IsValid(Enemy)) continue;

        float Dist = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
        if (Dist <= NearestDist)
        {
            NearestDist = Dist;
            NearestEnemy = Enemy;
        }
    }

    LockOnTarget = NearestEnemy;
}

void APlayerCharacter::UpdateLockOnRotation(float DeltaTime)
{
    if (!LockOnTarget) return;

    FVector TargetDir = LockOnTarget->GetActorLocation() - GetActorLocation();
    TargetDir.Z = 0.f;
    FRotator TargetRot = TargetDir.Rotation();

    FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 5.f);
    SetActorRotation(NewRot);

    if (Controller)
    {
        Controller->SetControlRotation(NewRot);
    }
}

void APlayerCharacter::SwitchTarget(bool bLeft)
{
    if (!bIsLockedOn || !LockOnTarget)
        return;

    FVector MyLocation = GetActorLocation();
    FVector MyForward = GetActorForwardVector();
    FVector MyRight = GetActorRightVector();

    float MinAngle = 180.f;
    AEnemyBase* NewTarget = nullptr;

    for (TActorIterator<AEnemyBase> It(GetWorld()); It; ++It)
    {
        AEnemyBase* Enemy = *It;
        if (!IsValid(Enemy) || Enemy == LockOnTarget)
            continue;

        FVector ToEnemy = Enemy->GetActorLocation() - MyLocation;
        ToEnemy.Z = 0.f;
        ToEnemy.Normalize();

        float DotRight = FVector::DotProduct(ToEnemy, MyRight);
        float DotForward = FVector::DotProduct(ToEnemy, MyForward);

        if (DotForward < 0.3f)
            continue;

        if ((bLeft && DotRight < 0.f) || (!bLeft && DotRight > 0.f))
        {
            float Angle = FMath::Acos(FVector::DotProduct(MyForward, ToEnemy)) * (180.f / PI);
            if (Angle < MinAngle)
            {
                MinAngle = Angle;
                NewTarget = Enemy;
            }
        }
    }

    if (NewTarget)
    {
        if (CurrentTargetEnemy)
        {
            CurrentTargetEnemy->ShowTargetMarker(false);
        }

        LockOnTarget = NewTarget;
        CurrentTargetEnemy = NewTarget;
        CurrentTargetEnemy->ShowTargetMarker(true);
    }
}

void APlayerCharacter::SwitchTargetLeft()
{
    if (IsAttacking() || IsDodging())
        return;

    SwitchTarget(true);
}

void APlayerCharacter::SwitchTargetRight()
{
    if (IsAttacking() || IsDodging())
        return;

    SwitchTarget(false);
}

void APlayerCharacter::CheckLockOnDistance()
{
    if (!bIsLockedOn || !LockOnTarget) return;

    float Dist = FVector::Dist(GetActorLocation(), LockOnTarget->GetActorLocation());
    if (Dist > LockOnBreakDistance)
    {
        if (CurrentTargetEnemy)
        {
            CurrentTargetEnemy->ShowTargetMarker(false);
        }

        bIsLockedOn = false;
        LockOnTarget = nullptr;
        CurrentTargetEnemy = nullptr;

        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
    }
}

FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
    return FGenericTeamId(TeamID);
}