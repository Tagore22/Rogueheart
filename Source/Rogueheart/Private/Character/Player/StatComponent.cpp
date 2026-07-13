/*#include "Character/Player/StatComponent.h"

void UStatComponent::SetPlayerState(EPlayerState NewState)
{
    CurrentState = NewState;
}

bool UStatComponent::CanAct(EActionType DesiredAction) const // s
{
    switch (CurrentState)
    {
    case EPlayerState::Idle:
    case EPlayerState::Moving:
        return true;
    case EPlayerState::Attacking:
    {
        if (DesiredAction == EActionType::Attack)
        {
            return true;
        }
        return false;
    }
    case EPlayerState::Dodging:
    case EPlayerState::Damaged:
    case EPlayerState::Dead:
        return false;
    }
    return false;
}

void UStatComponent::HealPlayer(float PlusHP)
{
    UE_LOG(LogTemp, Warning, TEXT("PrevHP : %f"), CurHP);
    CurHP = FMath::Min(CurHP + PlusHP, MaxHP);
    UE_LOG(LogTemp, Warning, TEXT("CurHP : %f"), CurHP);
}

void UStatComponent::OnActStart()
{
    ConsumeStamina(StaminaCost);
}

void UStatComponent::ConsumeStamina(float Cost)
{
    ARogueheartPlayerController* CachedController = Player->GetCachedController();
    if (!IsValid(CachedController))
    {
        return;
    }
    CurStamina = FMath::Clamp(CurStamina - Cost, 0.f, MaxStamina);

    CachedController->SetStaminaPercent(CurStamina / MaxStamina);
}

bool UStatComponent::CanPlusStamina() const
{
    if (CurStamina >= MaxStamina)
        return false;

    switch (CurrentState)
    {
    case EPlayerState::Idle:
    case EPlayerState::Moving:
    case EPlayerState::Damaged:
        return true;
    case EPlayerState::Attacking:
    case EPlayerState::Dodging:
    case EPlayerState::Dead:
        return false;
    }
    return false;
}*/