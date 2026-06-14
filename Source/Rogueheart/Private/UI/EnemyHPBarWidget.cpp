#include "UI/EnemyHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyHPBarWidget::SetHPPercent(float Percent)
{
	HPBar->SetPercent(Percent);
}

void UEnemyHPBarWidget::SetDamageSum(float Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetNameSafe(DamageText));

	DamageSum += Damage;
	DamageText->SetText(FText::AsNumber(FMath::RoundToInt(DamageSum)));
	DamageText->SetVisibility(ESlateVisibility::Visible);
}

void UEnemyHPBarWidget::ResetDamageSum()
{
	DamageText->SetVisibility(ESlateVisibility::Hidden);
	UE_LOG(LogTemp, Warning, TEXT("DamageTEXT Hidden"));
	DamageSum = 0.f;
}

/*void UEnemyHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DamageText->SetVisibility(ESlateVisibility::Hidden);
}*/

