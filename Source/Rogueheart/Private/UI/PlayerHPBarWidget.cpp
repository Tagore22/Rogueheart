#include "UI/PlayerHPBarWidget.h"
#include "Components/ProgressBar.h"

void UPlayerHPBarWidget::SetHPPercent(const float Percent)
{
	HPBar->SetPercent(Percent);
}

/*void UPlayerHPBarWidget::SetMPPercent(const float Percent)
{
	MPBar->SetPercent(Percent);
}*/

void UPlayerHPBarWidget::SetStaminaPercent(const float Percent)
{
	StaminaBar->SetPercent(Percent);
}

