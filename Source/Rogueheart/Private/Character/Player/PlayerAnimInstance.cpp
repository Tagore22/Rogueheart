#include "Character/Player/PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/PlayerCharacter.h"

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

    APlayerCharacter* PC = Cast<APlayerCharacter>(OwningPawn);
    if (!PC) return;

    if (PC->IsDodging())
        Speed = 0.f;
    else
        Speed = OwningPawn->GetVelocity().Size();
    UE_LOG(LogTemp, Log, TEXT("Speed : %f"), Speed);

    ACharacter* Character = Cast<ACharacter>(OwningPawn);
    if (Character)
    {
        bIsInAir = Character->GetCharacterMovement()->IsFalling();
        bIsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
    }
}

void UPlayerAnimInstance::AnimNotify_EndAttack()
{
    UE_LOG(LogTemp, Warning, TEXT("AnimNotify_EndAttack"));
    SetIsAttacking(false);
    ResetPlayerToIdle();
}

void UPlayerAnimInstance::AnimNotify_EndDodge()
{
    ResetPlayerToIdle();
}

void UPlayerAnimInstance::ResetPlayerToIdle()
{
    if (APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner()))
    {
        PC->SetPlayerState(EPlayerState::Idle);
    }
}