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

    // ȸ�� �߿� Speed ���� 0 ó��
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
        ResetPlayerToIdle();
        Player->bCanNextCombo = false;
        Player->bInputCombo = false;
    }
    SetIsAttacking(false);
}

void UPlayerAnimInstance::AnimNotify_NextCombo()
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(TryGetPawnOwner()))
    {
        if (Player->bInputCombo && Player->CurrentCombo < Player->MaxCombo)
        {
            Player->CurrentCombo++;
            if (Montage_IsPlaying(Player->AMT_Attack))
            {
                Montage_JumpToSection(FName(FString::Printf(TEXT("Attack_%d"), Player->CurrentCombo)), Player->AMT_Attack);
            }

            Player->bCanNextCombo = false;
            Player->bInputCombo = false;
        }
        else
        {
            Player->bCanNextCombo = true; // ���� �޺��� ���� �� ����
        }
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