#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StatSubsystem.generated.h"

UCLASS()
class ROGUEHEART_API UStatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void HealPlayer(float PlusHP);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHP() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurHP() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxStamina() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurStamina() const;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "HP")
	float MaxHP = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "HP")
	float CurHP = 50.f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "Stamina")
	float MaxStamina = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "Stamina")
	float CurStamina = 100.f;
};
