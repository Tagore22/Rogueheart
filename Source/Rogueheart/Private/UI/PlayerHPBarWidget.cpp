#include "UI/PlayerHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UPlayerHPBarWidget::SetHPPercent(const float Percent)
{
	HPBar->SetPercent(Percent);
}

void UPlayerHPBarWidget::SetMPPercent(const float Percent)
{
	MPBar->SetPercent(Percent);
}

void UPlayerHPBarWidget::SetStaminaPercent(const float Percent)
{
	StaminaBar->SetPercent(Percent);
}

void UPlayerHPBarWidget::SetTextBlock(int32 Num)
{
	if (!TextBlock)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nullptr!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Exist"));
	}
	TextBlock->SetText(FText::AsNumber(Num));
}

