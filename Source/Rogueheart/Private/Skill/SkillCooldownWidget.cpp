#include "Skill/SkillCooldownWidget.h"
#include "Components/ProgressBar.h"

void USkillCooldownWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // �ʿ��� �ʱ�ȭ ����
    if (PB_Cooldown)
    {
        PB_Cooldown->SetPercent(0.0f);
    }
}

void USkillCooldownWidget::OnSkillCooldownUpdated(ESkillType SkillType, float RemainingTime)
{
    if (!PB_Cooldown || MaxCooldown <= 0.f)
        return;

    float Percent = FMath::Clamp(RemainingTime / MaxCooldown, 0.f, 1.f);
    PB_Cooldown->SetPercent(Percent);
}