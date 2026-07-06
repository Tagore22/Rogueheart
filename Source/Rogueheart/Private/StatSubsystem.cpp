#include "StatSubsystem.h"

void UStatSubsystem::HealPlayer(float PlusHP) //
{
    UE_LOG(LogTemp, Warning, TEXT("PrevHP : %f"), CurHP);
    CurHP = FMath::Min(CurHP + PlusHP, MaxHP);
    UE_LOG(LogTemp, Warning, TEXT("CurHP : %f"), CurHP);
}

float UStatSubsystem::GetMaxHP() const // 
{
    return MaxHP;
}

float UStatSubsystem::GetCurHP() const // 
{
    return CurHP;
}

float UStatSubsystem::GetMaxStamina() const //
{
    return MaxStamina;
}

float UStatSubsystem::GetCurStamina() const // 
{
    return CurStamina;
}

