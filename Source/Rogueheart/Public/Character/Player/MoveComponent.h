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
	
	FVector GetLastMoveInput() const { return LastMoveInput; }
protected:
	void Move(const struct FInputActionValue& Value); //

protected:
	//FVector LastMoveInput = FVector::ZeroVector; //

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move; //

	FVector LastMoveInput;
};
