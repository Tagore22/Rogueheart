#include "Character/Player/PlayerAnimInstance.h"
#include "Character/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
    Speed = 0.f;
    bIsInAir = false;
    bIsAccelerating = false;
    bIsAttacking = false;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwningPawn = TryGetPawnOwner();
    if (!OwningPawn) return;

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwningPawn);
    if (!PlayerCharacter) return;

    // È¸ÇÇ ÁßÀÏ ¶© Speed = 0
    if (PlayerCharacter->IsDodging())
    {
        Speed = 0.f;
    }
    else
    {
        Speed = OwningPawn->GetVelocity().Size();
    }

    if (const UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement())
    {
        bIsInAir = Movement->IsFalling();
        bIsAccelerating = Movement->GetCurrentAcceleration().Size() > 0.f;
    }
}

void UPlayerAnimInstance::AnimNotify_EndAttack()
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner()))
    {
        Player->OnAttackEnd();
    }
    SetIsAttacking(false);
}

void UPlayerAnimInstance::AnimNotify_NextCombo()
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner()))
    {
        Player->bCanNextCombo = true;
    }
}

void UPlayerAnimInstance::AnimNotify_EndDodge()
{
    ResetPlayerToIdle();
}

void UPlayerAnimInstance::ResetPlayerToIdle()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner()))
    {
        PlayerCharacter->SetPlayerState(EPlayerState::Idle);
    }
}