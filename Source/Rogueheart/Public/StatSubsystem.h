#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StatSubsystem.generated.h"

UCLASS()
class ROGUEHEART_API UStatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//void HealPlayer(float PlusHP);

	float GetMaxHP() const { return MaxHP; }

	float GetCurHP() const { return CurHP; }

	float GetMaxMP() const { return MaxMP; }

	float GetCurMP() const { return CurMP; }

	float GetMaxStamina() const { return MaxStamina; }

	float GetCurStamina() const { return CurStamina; }

	int32 GetSoulSum() const { return SoulSum; }

	int32 GetHPLevel() const { return HPLevel; }

	int32 GetMPLevel() const { return MPLevel; }

	int32 GetStaminaLevel() const { return StaminaLevel; }

	void SetMaxHP(float NewMaxHP);

	void SetCurHP(float NewCurHP);

	void SetMaxMP(float NewMaxMP);

	void SetCurMP(float NewCurMP);

	void SetMaxStamina(float NewMaxStamina);

	void SetCurStamina(float NewCurStamina);

	void SetSoulSum(int32 NewSoul);

	void SetHPLevel(int32 NewHPLevel);

	void SetMPLevel(int32 NewMPLevel);

	void SetStaminaLevel(int32 NewStaminaLevel);

private:
	bool CheckStatValue(float Value) const { return Value <= 0.f; }

private:
	float MaxHP = 100.f;

	float CurHP = 100.f;

	float MaxMP = 100.f;

	float CurMP = 100.f;

	float MaxStamina = 100.f;

	float CurStamina = 100.f;

	int32 SoulSum = 0;

	int32 HPLevel = 0;

	int32 MPLevel = 0;

	int32 StaminaLevel = 0;
};
