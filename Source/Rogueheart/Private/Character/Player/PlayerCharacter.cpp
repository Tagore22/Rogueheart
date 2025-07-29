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
    }
}

// 락온 대상 바라보기
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
    FVector2D MovementVector = Value.Get<FVector2D>();

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
    if (!CanAct())
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

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
    if (!CanAct() || AMT_Dodge == nullptr)
        return;

    SetPlayerState(EPlayerState::Dodging);

    if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
    {
        Anim->Montage_Play(AMT_Dodge);
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

// 락온 토글
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

// 가장 가까운 타겟 찾기
void APlayerCharacter::FindNearestTarget()
{
    float NearestDist = LockOnRange;
    AActor* NearestEnemy = nullptr;

    for (TActorIterator<AEnemyBase> It(GetWorld()); It; ++It)
    {
        AEnemyBase* Enemy = *It;
        if (!Enemy || Enemy->IsPendingKill()) continue;

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