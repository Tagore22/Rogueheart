#include "Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Player/PlayerAnimInstance.h"
#include "Skill/SkillCooldownWidget.h"

APlayerCharacter::APlayerCharacter()
{
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

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
        EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
        EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
        EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
        EnhancedInput->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APlayerCharacter::Dodge);
        EnhancedInput->BindAction(IA_Skill1, ETriggerEvent::Started, this, &APlayerCharacter::UseFireball);
        EnhancedInput->BindAction(IA_Skill2, ETriggerEvent::Started, this, &APlayerCharacter::UseIceBlast);
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    UE_LOG(LogTemp, Log, TEXT("Move called: State=%s, Vec=(%f,%f)"),
        *UEnum::GetValueAsString(CurrentState),
        MovementVector.X, MovementVector.Y);
    if (!CanAct() || CurrentState == EPlayerState::Attacking || CurrentState == EPlayerState::Dodging)
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
    if (!CanAct()) return;
    FVector2D LookAxis = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxis.X * BaseTurnRate * GetWorld()->GetDeltaSeconds());
    AddControllerPitchInput(LookAxis.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
    if (!CanAct()) return;
    SetPlayerState(EPlayerState::Jumping);
    Jump();
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
    StopJumping();
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    SetPlayerState(EPlayerState::Idle);
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
    if (!CanAct() || CurrentState != EPlayerState::Idle || AMT_Attack == nullptr)
        return;
    SetPlayerState(EPlayerState::Attacking);
    if (UPlayerAnimInstance* Anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance()))
    {
        Anim->SetIsAttacking(true);
        Anim->Montage_Play(AMT_Attack);
        // 상태 복귀: 몽타주 끝나면 Notify에서 Idle로 전환 예정
    }
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
    if (!CanAct() || CurrentState != EPlayerState::Idle || AMT_Dodge == nullptr)
        return;
    SetPlayerState(EPlayerState::Dodging);
    LaunchCharacter(GetActorForwardVector() * DodgeSpeed, true, true);
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
    return CurrentState != EPlayerState::Attacking && CurrentState != EPlayerState::Dodging && CurrentState != EPlayerState::Stunned;
}
