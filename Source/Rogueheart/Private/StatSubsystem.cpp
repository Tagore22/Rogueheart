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

void UStatSubsystem::SetMaxMP(float NewMaxMP)
{
    if (CheckStatValue(NewMaxMP))
    {
        return;
    }

    MaxMP = NewMaxMP;
    UE_LOG(LogTemp, Warning, TEXT("MaxMP : %f"), NewMaxMP);
}

void UStatSubsystem::SetCurMP(float NewCurMP)
{
    if (CheckStatValue(NewCurMP))
    {
        return;
    }

    CurMP = NewCurMP;
    UE_LOG(LogTemp, Warning, TEXT("CurMP : %f"), CurMP);
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

void UStatSubsystem::SetSoulSum(int32 NewSoul)
{
    if (NewSoul < 0)
    {
        return;
    }
    SoulSum = NewSoul;
    UE_LOG(LogTemp, Warning, TEXT("SoulSum : %d"), SoulSum);
}

void UStatSubsystem::SetHPLevel(int32 NewHPLevel)
{
    if (NewHPLevel < 0)
    {
        return;
    }
    HPLevel = NewHPLevel;
    UE_LOG(LogTemp, Warning, TEXT("HPLevel : %d"), HPLevel);
}

void UStatSubsystem::SetMPLevel(int32 NewMPLevel)
{
    if (NewMPLevel < 0)
    {
        return;
    }
    MPLevel = NewMPLevel;
    UE_LOG(LogTemp, Warning, TEXT("MPLevel : %d"), MPLevel);
}

void UStatSubsystem::SetStaminaLevel(int32 NewStaminaLevel)
{
    if (NewStaminaLevel < 0)
    {
        return;
    }
    StaminaLevel = NewStaminaLevel;
    UE_LOG(LogTemp, Warning, TEXT("StaminaLevel : %d"), StaminaLevel);
}

