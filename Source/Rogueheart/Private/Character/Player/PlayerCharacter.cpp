#include "Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Player/PlayerAnimInstance.h"  

/*APlayerCharacter::APlayerCharacter()
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
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    UE_LOG(LogTemp, Warning, TEXT("Move Input -> X: %f, Y: %f"), MovementVector.X, MovementVector.Y);

    if (Controller && (MovementVector.X != 0.f || MovementVector.Y != 0.f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.X);
        AddMovementInput(RightDirection, MovementVector.Y);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    AddControllerYawInput(LookAxisVector.X * BaseTurnRate * GetWorld()->GetDeltaSeconds());
    AddControllerPitchInput(LookAxisVector.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::StartJump(const FInputActionValue& Value)
{
    Jump();
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
    StopJumping();
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Attack"));
    if (AMT_Attack == nullptr) 
        return;

    UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
    if (AnimInst && AMT_Attack)
    {
        // 공격 상태 true 세팅
        AnimInst->SetIsAttacking(true);
        // 몽타주 재생
        AnimInst->Montage_Play(AMT_Attack);
    }
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Dodge"));
    if (Controller == nullptr || AMT_Dodge == nullptr)
        return;

    FVector DodgeDirection = GetActorForwardVector();
    LaunchCharacter(DodgeDirection * DodgeSpeed, true, true);

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(AMT_Dodge);
    }
}*/