#pragma once

#include "CoreMinimal.h"
#include "Character/Player/PlayerBaseComponent.h"
#include "MoveComponent.generated.h"

UCLASS()
class ROGUEHEART_API UMoveComponent : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	virtual void SetupInputBinding(UEnhancedInputComponent* EnhancedInput) override;
	
protected:
	void Move(const struct FInputActionValue& Value);

	void Look(const struct FInputActionValue& Value);

	void Dodge(const struct FInputActionValue& Value);

protected:
	FVector LastMoveInput = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Dodge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* AMT_Dodge;
};
