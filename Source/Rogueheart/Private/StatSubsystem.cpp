#include "StatSubsystem.h"

/*void UStatSubsystem::HealPlayer(float PlusHP) //
{
    UE_LOG(LogTemp, Warning, TEXT("PrevHP : %f"), CurHP);
    CurHP = FMath::Min(CurHP + PlusHP, MaxHP);
    UE_LOG(LogTemp, Warning, TEXT("CurHP : %f"), CurHP);
}*/

void UStatSubsystem::SetMaxHP(float NewMaxHP) 
{
    if (!CheckStatValue(NewMaxHP))
    {
        return;
    }

    MaxHP = NewMaxHP;
}

void UStatSubsystem::SetCurHP(float NewCurHP) 
{
    if (!CheckStatValue(NewCurHP))
    {
        return;
    }

    CurHP = NewCurHP;
}

void UStatSubsystem::SetMaxStamina(float NewMaxStamina) 
{
    if (!CheckStatValue(NewMaxStamina))
    {
        return;
    }

    MaxStamina = NewMaxStamina;
}

void UStatSubsystem::SetCurStamina(float NewCurStamina)
{
    if (!CheckStatValue(NewCurStamina))
    {
        return;
    }

    CurStamina = NewCurStamina;
}

