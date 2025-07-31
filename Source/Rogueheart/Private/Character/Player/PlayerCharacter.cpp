#include "Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Player/PlayerAnimInstance.h"
#include "Skill/SkillCooldownWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Enemy/EnemyBase.h"
#include "EngineUtils.h"
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

    CurrentState = EPlayerState::Idle;
    CurrentCombo = 0;
    MaxCombo = 3;
    bInputCombo = false;
    bCanNextCombo = false;
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
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector MovementVector = Value.Get<FVector>();
    LastMoveInput = MovementVector;  // 입력 저장

    if (!CanAct())
        return;

    if (Controller && (MovementVector.X != 0.f || MovementVector.Y != 0.f))
    {
        const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), MovementVector.X);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), MovementVector.Y);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    if (CurrentState == EPlayerState::Dodging || CurrentState == EPlayerState::Stunned)
        return;

    if (bIsLockedOn)
        return;

    FVector2D LookAxis = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxis.X * BaseTurnRate * GetWorld()->GetDeltaSeconds());
    AddControllerPitchInput(LookAxis.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
    if (CurrentState == EPlayerState::Dodging || CurrentState == EPlayerState::Stunned)
        return;

    if (CurrentState != EPlayerState::Attacking)
    {
        CurrentCombo = 1;
        bInputCombo = false;
        bCanNextCombo = false;

        PlayComboMontage();
        SetPlayerState(EPlayerState::Attacking);
    }
    else if (bCanNextCombo)
    {
        bInputCombo = true;
    }
}

void APlayerCharacter::PlayComboMontage()
{
    if (UPlayerAnimInstance* Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance()))
    {
        if (AMT_Attack)
        {
            Anim->Montage_Play(AMT_Attack);
            Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("Combo%d"), CurrentCombo)), AMT_Attack);
        }
    }
}

void APlayerCharacter::HandleComboInput()
{
    if (CurrentCombo >= MaxCombo)
        return;

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

void APlayerCharacter::RestoreLockOnIfNeeded()
{
    // 회피 전 타겟팅 상태였던 경우 다시 타겟팅
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

    // 회피 시작 시 현재 타겟팅 상태 저장
    bWasLockedOnWhenDodged = bIsLockedOn;

    // 입력 방향으로 캐릭터 회전
    if (!LastMoveInput.IsNearlyZero())
    {
        FRotator ControlRot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
        FVector DodgeDir = ControlRot.RotateVector(LastMoveInput);
        FRotator LookAtRot = (DodgeDir + GetActorLocation() - GetActorLocation()).Rotation();
        SetActorRotation(FRotator(0.f, LookAtRot.Yaw, 0.f));
    }

    // 회피 애니메이션 재생
    if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
    {
        Anim->Montage_Play(AMT_Dodge);
    }

    // 회피 중에는 타겟팅 해제
    if (bIsLockedOn)
    {
        bIsLockedOn = false;
        LockOnTarget = nullptr;
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
    return CurrentState != EPlayerState::Attacking &&
        CurrentState != EPlayerState::Dodging &&
        CurrentState != EPlayerState::Stunned;
}

void APlayerCharacter::ToggleLockOn()
{
    UE_LOG(LogTemp, Warning, TEXT("LockOn"));
    if (bIsLockedOn)
    {
        bIsLockedOn = false;
        LockOnTarget = nullptr;
        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
    }
    else
    {
        FindNearestTarget();
        if (LockOnTarget)
        {
            bIsLockedOn = true;
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
    AActor* NewTarget = nullptr;

    for (TActorIterator<AEnemyBase> It(GetWorld()); It; ++It)
    {
        AEnemyBase* Enemy = *It;
        if (!Enemy || Enemy == LockOnTarget || Enemy->IsPendingKill())
            continue;

        FVector ToEnemy = Enemy->GetActorLocation() - MyLocation;
        ToEnemy.Z = 0.f;
        ToEnemy.Normalize();

        float DotRight = FVector::DotProduct(ToEnemy, MyRight);
        float DotForward = FVector::DotProduct(ToEnemy, MyForward);

        // 일정 각도 내 전방에서만 전환 허용
        if (DotForward < 0.3f) // 너무 뒤쪽은 제외
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
        LockOnTarget = NewTarget;
    }
}