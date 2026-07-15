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

	float GetMaxStamina() const { return MaxStamina; }

	float GetCurStamina() const { return CurStamina; }

	void SetMaxHP(float NewMaxHP);

	void SetCurHP(float NewCurHP);

	void SetMaxStamina(float NewMaxStamina);

	void SetCurStamina(float NewCurStamina);

private:
	bool CheckStatValue(float Value) const { return Value <= 0; }

private:
	float MaxHP = 100.f;

	float CurHP = 100.f;

	float MaxStamina = 100.f;

	float CurStamina = 100.f;
};
