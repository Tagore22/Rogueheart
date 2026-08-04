#include "UI/PlayerHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UPlayerHPBarWidget::SetHPPercent(float Percent)
{
	HPBar->SetPercent(Percent);
}

void UPlayerHPBarWidget::SetMPPercent(float Percent)
{
	MPBar->SetPercent(Percent);
}

void UPlayerHPBarWidget::SetStaminaPercent(float Percent)
{
	StaminaBar->SetPercent(Percent);
}

void UPlayerHPBarWidget::HPWidthOverride(float Width)
{
	HPBox->SetWidthOverride(Width * WidthRatio);
	UE_LOG(LogTemp, Warning, TEXT("HPBox Width : %f"), HPBox->GetWidthOverride());
}

void UPlayerHPBarWidget::MPWidthOverride(float Width)
{
	MPBox->SetWidthOverride(Width * WidthRatio);
}

void UPlayerHPBarWidget::StaminaWidthOverride(float Width)
{
	StaminaBox->SetWidthOverride(Width * WidthRatio);
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

void UPlayerHPBarWidget::UIWidthInitialize(float MaxHP, float MaxMP, float MaxStamina)
{
	HPWidthOverride(MaxHP);
	HPBox->SetHeightOverride(Height);

	MPWidthOverride(MaxMP);
	MPBox->SetHeightOverride(Height);

	StaminaWidthOverride(MaxStamina);
	StaminaBox->SetHeightOverride(Height);
}

