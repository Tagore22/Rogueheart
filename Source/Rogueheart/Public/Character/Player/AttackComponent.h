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

	void OnAttackEnd(); //

	void SetCanNextComboTrue(); //
protected:
	virtual void BeginPlay() override;

	void Attack(const struct FInputActionValue& Value); // 

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Attack; //

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TArray<class UAnimMontage*> AttackMontages; //

	int32 CurrentCombo = 0; //

	bool bInputCombo = false; //

	bool bCanNextCombo = false; //

	int32 MaxCombo; //
};
