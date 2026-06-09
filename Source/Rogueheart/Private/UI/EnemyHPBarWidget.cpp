#include "UI/EnemyHPBarWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHPBarWidget::SetHPPercent(const float Percent)
{
	HPBar->SetPercent(Percent);
}

