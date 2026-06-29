#pragma once

#include "CoreMinimal.h"
#include "Character/Player/PlayerBaseComponent.h"
#include "TargetComponent.generated.h"

UCLASS()
class ROGUEHEART_API UTargetComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputBinding(class UEnhancedInputComponent* EnhancedInput);

	void UpdateLockOnRotation(float DeltaTime);

	void ClearLockOn();

	class AEnemyBase* FindNearestTarget();

	class AEnemyBase* SwitchTarget(bool bLeft);

protected:
	void ToggleLockOn(const struct FInputActionValue& Value);

	void SwitchTargetLeft(const struct FInputActionValue& Value);

	void SwitchTargetRight(const struct FInputActionValue& Value);

	void SetLockOnTarget(class AEnemyBase* NewTarget);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	int32 InterpSpeed = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float LockOnRange = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetingAngle = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LockOn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SwitchTargetLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SwitchTargetRight;
};
