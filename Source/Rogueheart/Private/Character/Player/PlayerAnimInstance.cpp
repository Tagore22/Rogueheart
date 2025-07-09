#include "Character/Player/PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

/*UPlayerAnimInstance::UPlayerAnimInstance()
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

    Speed = OwningPawn->GetVelocity().Size();

    ACharacter* Character = Cast<ACharacter>(OwningPawn);
    if (Character)
    {
        bIsInAir = Character->GetCharacterMovement()->IsFalling();
        bIsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
    }
}

void UPlayerAnimInstance::AnimNotify_EndAttack()
{
    SetIsAttacking(false);
}*/