#include "StatSubsystem.h"

/*void UStatSubsystem::HealPlayer(float PlusHP) //
{
    UE_LOG(LogTemp, Warning, TEXT("PrevHP : %f"), CurHP);
    CurHP = FMath::Min(CurHP + PlusHP, MaxHP);
    UE_LOG(LogTemp, Warning, TEXT("CurHP : %f"), CurHP);
}*/

void UStatSubsystem::SetMaxHP(float NewMaxHP) 
{
    if (CheckStatValue(NewMaxHP))
    {
        return;
    }

    MaxHP = NewMaxHP;
    UE_LOG(LogTemp, Warning, TEXT("MaxHP : %f"), NewMaxHP);
}

void UStatSubsystem::SetCurHP(float NewCurHP) 
{
    if (CheckStatValue(NewCurHP))
    {
        return;
    }

    CurHP = NewCurHP;
    UE_LOG(LogTemp, Warning, TEXT("CurHP : %f"), CurHP);
}

void UStatSubsystem::SetMaxStamina(float NewMaxStamina) 
{
    if (CheckStatValue(NewMaxStamina))
    {
        return;
    }

    MaxStamina = NewMaxStamina;
    UE_LOG(LogTemp, Warning, TEXT("MaxStamina : %f"), MaxStamina);
}

void UStatSubsystem::SetCurStamina(float NewCurStamina)
{
    if (CheckStatValue(NewCurStamina))
    {
        return;
    }

    CurStamina = NewCurStamina;
    UE_LOG(LogTemp, Warning, TEXT("CurStamina : %f"), CurStamina);
}

