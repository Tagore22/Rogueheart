#include "Character/Player/PlayerAnimInstance.h"
#include "Character/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
    Speed = 0.f;
    bIsInAir = false;
    bIsAccelerating = false;
    bIsAttacking = false;

    WalkSpeed = 0.f;
    Direct = 0.f;
    bIsLockedOn = false;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwningPawn = TryGetPawnOwner();
    if (!OwningPawn) return;

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwningPawn);
    if (!PlayerCharacter) return;

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

    // 락온 상태 확인
    bIsLockedOn = PlayerCharacter->bIsLockedOn;

    if (bIsLockedOn)
    {
        FVector Velocity = OwningPawn->GetVelocity();
        FVector Forward = OwningPawn->GetActorForwardVector();
        FVector Right = OwningPawn->GetActorRightVector();

        FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
        WalkSpeed = FVector::DotProduct(HorizontalVelocity, Forward);
        Direct = FVector::DotProduct(HorizontalVelocity, Right);
    }
    else
    {
        WalkSpeed = 0.f;
        Direct = 0.f;
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