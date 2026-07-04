#include "Character/Player/AttackComponent.h"

void UAttackComponent::SetupInputBinding(UEnhancedInputComponent* EnhancedInput)
{
    Super::SetupInputBinding(EnhancedInput);

    EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &UAttackComponent::Attack);
}

void UAttackComponent::PlayComboMontage() //
{
    UAnimInstance* Anim = Player->GetMesh()->GetAnimInstance();
    /*if (!Anim || !AMT_Attack)
        return;

    Anim->Montage_Play(AMT_Attack);
    Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("Combo%d"), CurrentCombo)), AMT_Attack);*/

    //
    if (!Anim || AttackMontages.Num() == 0)
        return;

    Anim->Montage_Play(AttackMontages[CurrentCombo - 1]);
}

void UAttackComponent::OnAttackEnd()
{
    if (bInputCombo && CurrentCombo < MaxCombo)
    {
        ++CurrentCombo;
        PlayComboMontage();
    }
    else
    {
        CurrentCombo = 0;
        Player->SetPlayerState(EPlayerState::Idle);
    }
    bInputCombo = false;
    bCanNextCombo = false;
}

void UAttackComponent::SetCanNextComboTrue() //
{
    bCanNextCombo = true;
}

void UAttackComponent::Attack(const FInputActionValue& Value) //
{
    if (!Player->CanAct(EActionType::Attack) || Player->GetCurStamina() <= 0.f) // 스테미나 값을 반환하는 public 함수를 사용.
        return;

    if (!Player->IsAttacking())
    {
        CurrentCombo = 1;
        bInputCombo = false;
        // 공격 애니메이션 도중 NextCombo 노티파이에 의해서만 true로 바뀜.
        bCanNextCombo = false;

        Player->SetPlayerState(EPlayerState::Attacking);
        PlayComboMontage();
    }
    else if (bCanNextCombo && !bInputCombo)
    {
        bInputCombo = true;
    }
}