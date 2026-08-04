#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPBarWidget.generated.h"

UCLASS()
class ROGUEHEART_API UPlayerHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHPPercent(float Percent);

	void SetMPPercent(float Percent);

	void SetStaminaPercent(float Percent);

	void HPWidthOverride(float Width);

	void MPWidthOverride(float Width);

	void StaminaWidthOverride(float Width);

	void SetTextBlock(int32 Num);

	void UIWidthInitialize(float MaxHP, float MaxMP, float MaxStamina);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MPBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* HPBox;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* MPBox;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* StaminaBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock;

	UPROPERTY(EditDefaultsOnly)
	float WidthRatio = 8.5;

	UPROPERTY(EditDefaultsOnly)
	float Height = 70.f;
};
