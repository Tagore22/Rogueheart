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

	void SetTextBlock(int32 Num);
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MPBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	class UBorder* Border;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock;
};
