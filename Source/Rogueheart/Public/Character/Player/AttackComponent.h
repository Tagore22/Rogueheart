#pragma once

#include "CoreMinimal.h"
#include "Character/Player/PlayerBaseComponent.h"
#include "AttackComponent.generated.h"

UCLASS()
class ROGUEHEART_API UAttackComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputBinding(UEnhancedInputComponent* EnhancedInput) override;

	void PlayComboMontage(); //
protected:
	void Attack(const struct FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Attack; //

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	int32 CurrentCombo = 0; //

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bInputCombo = false; //

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bCanNextCombo = false; //

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TArray<class UAnimMontage*> AttackMontages; //
};
